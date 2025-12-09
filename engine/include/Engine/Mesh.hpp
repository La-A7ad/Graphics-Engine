#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace engine {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds);
    void Draw(unsigned int shaderProgram) const;   

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

} // namespace engine
