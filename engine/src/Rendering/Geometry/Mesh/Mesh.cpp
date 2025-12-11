#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include <glad/glad.h>

namespace engine {

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
    : vertices(std::move(verts)), indices(std::move(inds)), 
      setupComplete(false), lastShader(nullptr) {
    
    // FIXED: Setup immediately in constructor using your Buffer abstractions
    // No need for lazy initialization or shader tracking
    setupMesh();
}

Mesh::~Mesh() {
    // VBO, VAO, EBO destructors handle cleanup automatically
}

void Mesh::setupMesh() {
    // Bind VAO first - all subsequent buffer operations are recorded
    vao.Bind();

    // Upload vertex data using YOUR VBO abstraction
    vbo.SetData(vertices.data(), vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

    // Upload index data using YOUR EBO abstraction  
    ebo.SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

    // IMPORTANT: We DON'T call AddAttribute here anymore
    // That requires a shader, which we don't have yet
    // The VAO just records the buffer bindings, attributes are set per-shader
    
    vao.Unbind();
    setupComplete = true;
}

void Mesh::Draw(const Shader& shader) {
    // FIXED: Setup attributes for this shader if not done yet
    // This happens once per unique shader used with this mesh
    if (lastShader != &shader) {
        vao.Bind();
        vbo.Bind(); // Re-bind VBO so attribute pointers reference it
        
        GLsizei stride = sizeof(Vertex);
        
        // Use YOUR Shader's attribute reflection system
        vao.AddAttribute(shader, "aPosition", stride, 0);
        vao.AddAttribute(shader, "aNormal", stride, offsetof(Vertex, Normal));
        vao.AddAttribute(shader, "aTexCoords", stride, offsetof(Vertex, TexCoords));
        
        vao.Unbind();
        lastShader = &shader;
    }
    
    // Draw using your abstractions
    shader.use();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

} // namespace engine