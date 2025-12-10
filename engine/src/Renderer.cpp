#include "Engine/Renderer.hpp"
#include "Engine/Transform.hpp" // Now just GLM wrappers
#include <iostream>

namespace engine {

Renderer::Renderer() 
    : shader(nullptr), width(800), height(600), initialized(false) {
}

Renderer::~Renderer() {
    delete shader;
}

bool Renderer::Init() {
    // Triangle vertex data (position only)
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // FIXED: Load shader first so attribute reflection works
    shader = new Shader("game/assets/shaders/basic.vert", 
                       "game/assets/shaders/basic.frag");

    // Setup VAO with YOUR Buffer abstractions
    vao.Bind();
    
    // Upload data using YOUR VBO
    vbo.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

    // Configure attributes using YOUR Shader reflection + VAO integration
    GLsizei stride = 3 * sizeof(float);
    vao.AddAttribute(*shader, "aPos", stride, 0);

    vao.Unbind();

    // Setup OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initialized = true;
    return true;
}

void Renderer::RenderFrame(float time) {
    if (!initialized) return;

    glClearColor(0.18f, 0.23f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use YOUR Shader abstraction
    shader->use();

    // FIXED: Use GLM (via Transform.hpp) for transforms
    mat4 model = glm::rotate(glm::mat4(1.0f), time * 0.5f, vec3(0, 0, 1));
    mat4 view = glm::mat4(1.0f);
    mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    // Set uniforms using YOUR Shader's methods
    shader->setMat4("uModel", glm::value_ptr(model));
    shader->setMat4("uView", glm::value_ptr(view));
    shader->setMat4("uProj", glm::value_ptr(proj));

    // Draw using YOUR VAO abstraction
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    vao.Unbind();
}

void Renderer::Resize(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

} // namespace engine