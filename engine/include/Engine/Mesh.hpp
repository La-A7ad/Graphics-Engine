#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Engine/Shader.hpp"
#include "Engine/Buffers.hpp"

namespace engine {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// FIXED: Mesh now properly uses YOUR Buffer abstractions
// Setup happens in constructor, attributes are configured per-shader
class Mesh {
public:
    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds);
    ~Mesh();
    
    // Draw with any shader - attributes are setup on first use with each shader
    void Draw(const Shader& shader);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // YOUR buffer abstractions - they handle OpenGL object lifetime
    VAO vao;
    VBO vbo;
    EBO ebo;
    
    // Track which shader we last configured attributes for
    // This allows using the same mesh with different shaders efficiently
    bool setupComplete;
    const Shader* lastShader;
    
    void setupMesh();
};

} // namespace engine