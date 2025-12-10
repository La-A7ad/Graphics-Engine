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
    
    bool setupComplete;
    const Shader* lastShader;
    
    void setupMesh(const Shader& shader);
};

} // namespace engine