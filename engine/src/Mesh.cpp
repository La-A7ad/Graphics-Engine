#include "Engine/Mesh.hpp"
#include <glad/glad.h>

namespace engine {

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
    : vertices(std::move(verts)), indices(std::move(inds)), 
      setupComplete(false), lastShader(nullptr) {
}

Mesh::~Mesh() {
}

void Mesh::setupMesh(const Shader& shader) {
    vao.Bind();

    vbo.SetData(vertices.data(), vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

    ebo.SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    GLsizei stride = sizeof(Vertex);
    
    vao.AddAttribute(shader, "aPosition", stride, 0);
    vao.AddAttribute(shader, "aNormal", stride, offsetof(Vertex, Normal));
    vao.AddAttribute(shader, "aTexCoords", stride, offsetof(Vertex, TexCoords));

    vao.Unbind();
    
    setupComplete = true;
    lastShader = &shader;
}

void Mesh::Draw(const Shader& shader) {
    // FIX: Only setup once - VAO configuration is shader-independent
    // The attribute locations are queried from the shader, but once bound to the VAO,
    // they don't need to be re-bound unless we're using a different shader with 
    // completely different attribute layouts
    if (!setupComplete) {
        setupMesh(shader);
    }
    
    shader.use();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

} // namespace engine