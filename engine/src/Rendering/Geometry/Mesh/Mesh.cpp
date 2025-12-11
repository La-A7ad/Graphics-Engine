#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include <glad/glad.h>

namespace engine {

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
    : vertices(std::move(verts)), indices(std::move(inds)) {
    
    // Bind VAO to record all subsequent buffer operations
    vao.Bind();

    // Upload vertex data to GPU using RAII VBO wrapper
    vbo.SetData(vertices.data(), 
                vertices.size() * sizeof(Vertex), 
                GL_STATIC_DRAW);

    // Upload index data to GPU using RAII EBO wrapper
    ebo.SetData(indices.data(), 
                indices.size() * sizeof(unsigned int), 
                GL_STATIC_DRAW);

    // Unbind VAO to prevent accidental modification during initialization
    // This is safe here because we're not in the render loop
    vao.Unbind();
}

Mesh::~Mesh() {
    // RAII: VAO, VBO, and EBO destructors handle OpenGL cleanup automatically
}

void Mesh::configureForShader(const Shader& shader) {
    GLuint shaderID = shader.getID();
    
    // Check if we've already configured attributes for this shader
    if (configuredShaders.find(shaderID) != configuredShaders.end()) {
        return; // Already configured - skip
    }
    
    // Configure vertex attributes for this shader
    // This is done once per unique shader that uses this mesh
    vao.Bind();
    vbo.Bind(); // Must bind VBO so attribute pointers reference it
    
    GLsizei stride = sizeof(Vertex);
    
    // Configure attributes using shader's reflection system
    // AddAttribute will check if the attribute exists in the shader
    vao.AddAttribute(shader, "aPosition", stride, 0);
    vao.AddAttribute(shader, "aNormal", stride, offsetof(Vertex, Normal));
    vao.AddAttribute(shader, "aTexCoords", stride, offsetof(Vertex, TexCoords));
    
    // Unbind to prevent accidental modification
    vao.Unbind();
    
    // Mark this shader as configured
    configuredShaders.insert(shaderID);
}

void Mesh::Draw(const Shader& shader) {
    // Lazy configuration: set up attributes if this is first use with this shader
    // This maintains flexibility while minimizing per-frame overhead
    configureForShader(shader);
    
    // Activate shader program
    shader.use();
    
    // Bind VAO (contains all vertex attribute state for this mesh)
    vao.Bind();
    
    // Issue draw call
    // VAO already has EBO bound, so indices come from there
    glDrawElements(GL_TRIANGLES, 
                   static_cast<GLsizei>(indices.size()), 
                   GL_UNSIGNED_INT, 
                   0);
    
    // Note: We don't unbind here for performance
    // The next draw call will bind its own VAO anyway
    // Unbinding is only necessary during initialization or when
    // you want to prevent accidental modification
}

} // namespace engine