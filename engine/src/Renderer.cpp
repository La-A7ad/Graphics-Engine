#include "Engine/Renderer.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

Renderer::Renderer() 
    : shader(nullptr), width(800), height(600), initialized(false) {
}

Renderer::~Renderer() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
}

bool Renderer::Init() {
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // Load shader first so we can use it for attribute reflection
    shader = new Shader("game/assets/shaders/basic.vert", 
                       "game/assets/shaders/basic.frag");

    // Bind VAO
    vao.Bind();

    // Upload vertex data using your VBO
    vbo.SetData(vertices, sizeof(vertices), GL_STATIC_DRAW);

    // Set up vertex attributes using your VAO::AddAttribute
    GLsizei stride = 3 * sizeof(float);
    vao.AddAttribute(*shader, "aPos", stride, 0);

    vao.Unbind();

    // FIX: Enable depth testing since we're clearing the depth buffer
    glEnable(GL_DEPTH_TEST);

    initialized = true;
    return true;
}

void Renderer::RenderFrame(float time) {
    if (!initialized) return;

    glClearColor(0.18f, 0.23f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // Set up transformation matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, time * 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    // Set uniforms using shader methods
    shader->setMat4("uModel", glm::value_ptr(model));
    shader->setMat4("uView", glm::value_ptr(view));
    shader->setMat4("uProj", glm::value_ptr(proj));

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