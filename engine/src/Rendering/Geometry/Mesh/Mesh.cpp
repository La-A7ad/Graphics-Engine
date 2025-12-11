#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include <glad/glad.h>
#include <iostream>

namespace engine {

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
    : vertices(std::move(verts)), indices(std::move(inds)) {
    setupMesh();
}

Mesh::~Mesh() {
    // RAII: VAO, VBO, EBO destructors handle cleanup automatically
}

void Mesh::setupMesh() {
    // Step 1: Bind VAO (captures subsequent state)
    vao.Bind();
    
    // Step 2: Upload vertex data
    // VBO must be bound BEFORE setting data
    vbo.Bind();  // Implicitly called by SetData, but being explicit
    vbo.SetData(vertices.data(), vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    
    // Step 3: Upload index data
    // EBO bound while VAO is bound - VAO will remember this binding
    ebo.Bind();  // Implicitly called by SetData
    ebo.SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    
    // Step 4: Unbind VAO (saves state)
    vao.Unbind();
    
    // NOTE: We don't configure attributes here because we don't have a shader yet
    // Attributes will be configured per-shader on first use
}

void Mesh::configureAttributesForShader(const Shader& shader) {
    // CRITICAL: VAO must be bound first
    vao.Bind();
    
    // CRITICAL: VBO must be bound so attribute pointers reference it
    vbo.Bind();
    
    GLsizei stride = sizeof(Vertex);
    
    // Use YOUR reflection system - check if attribute exists before configuring
    const auto* posAttr = shader.getAttrib("aPosition");
    if (posAttr) {
        vao.AddAttribute(shader, "aPosition", stride, 0);
    } else {
        std::cerr << "Warning: Shader missing 'aPosition' attribute\n";
    }
    
    const auto* normAttr = shader.getAttrib("aNormal");
    if (normAttr) {
        vao.AddAttribute(shader, "aNormal", stride, offsetof(Vertex, Normal));
    }
    
    const auto* texAttr = shader.getAttrib("aTexCoords");
    if (texAttr) {
        vao.AddAttribute(shader, "aTexCoords", stride, offsetof(Vertex, TexCoords));
    }
    
    // Unbind to prevent accidental modifications
    vao.Unbind();
    vbo.Unbind();
}

void Mesh::Draw(const Shader& shader) {
    // Check if we've configured attributes for this shader
    GLuint shaderID = shader.getID();
    
    if (configuredShaders.find(shaderID) == configuredShaders.end()) {
        // First time using this shader - configure attributes
        configureAttributesForShader(shader);
        configuredShaders[shaderID] = true;
    }
    
    // Now draw
    shader.use();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

}