#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_set>
#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include "Engine/Core/Graphics/Buffers/Buffers.hpp"

namespace engine {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

/**
 * Mesh class - Manages vertex data and rendering using RAII principles
 * 
 * - Uses VAO/VBO/EBO abstractions for automatic OpenGL resource management
 * - Lazily configures vertex attributes per shader (once per unique shader)
 * - Complies with OpenGL 3.3 Core Profile requirements
 */
class Mesh {
public:
    /**
     * Constructor - Uploads vertex and index data to GPU
     * @param verts Vertex data (position, normal, texcoords)
     * @param inds Index data for indexed drawing
     */
    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds);
    
    /**
     * Destructor - RAII handles cleanup automatically through member destructors
     */
    ~Mesh();
    
    /**
     * Draw the mesh using the specified shader
     * - Lazily configures VAO attributes for this shader (only once)
     * - Binds VAO and issues draw call
     * @param shader The shader program to use for rendering
     */
    void Draw(const Shader& shader);


    // Move Constructor (Defaults to moving the underlying VAO/VBO/EBO)
    Mesh(Mesh&& other) noexcept = default;
    
    // Delete Copy
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

private:
    // Vertex and index data (kept for potential future use)
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // OpenGL buffer objects - RAII managed
    VAO vao;
    VBO vbo;
    EBO ebo;
    
    // Track which shaders have been configured with this mesh
    // Using shader ID avoids pointer lifetime issues
    std::unordered_set<GLuint> configuredShaders;
    void configureForShader(const Shader& shader);
};

} // namespace engine