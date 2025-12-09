#include "renderer.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::~Renderer() { cleanup(); }

bool Renderer::init() {
    
    float vertices[] = {
        
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // create VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    // load shader
    if (!shader.loadFromFiles("../shaders/basic.vert", "../shaders/basic.frag")) {
        std::cerr << "Failed to load shaders\n";
        return false;
    }

    initialized = true;
    return true;
}

void Renderer::renderFrame(float time) {
    if (!initialized) return;
    glClearColor(0.18f, 0.23f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, time * 0.5f, glm::vec3(0,0,1)); 
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::ortho(-1.0f,1.0f,-1.0f,1.0f, -1.0f, 1.0f);

    glUniformMatrix4fv(shader.getUniformLocation("uModel"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(shader.getUniformLocation("uView"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(shader.getUniformLocation("uProj"), 1, GL_FALSE, &proj[0][0]);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Renderer::cleanup() {
    if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
    if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
}
