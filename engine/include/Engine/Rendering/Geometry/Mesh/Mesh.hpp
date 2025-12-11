#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include "Engine/Shader.hpp"
#include "Engine/Buffers.hpp"

namespace engine {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds);
    ~Mesh();
    
    void Draw(const Shader& shader);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    VAO vao;
    VBO vbo;
    EBO ebo;
    
    // Cache VAO configurations per shader ID (not pointer!)
    std::unordered_map<GLuint, bool> configuredShaders;
    
    void setupMesh();
    void configureAttributesForShader(const Shader& shader);
};

}