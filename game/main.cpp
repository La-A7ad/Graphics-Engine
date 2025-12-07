#include <glad/glad.h>      // MUST be first OpenGL header
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include "Engine/Shader.hpp"
#include "Engine/Buffers.hpp"



// Resize callback
static void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Request OpenGL 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    int width  = 1280;
    int height = 720;
    GLFWwindow* window = glfwCreateWindow(width, height, "GraphicsEngine Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Make context current
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Set viewport + callback
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // note to self: keep vertices and indices in main for now
    float Vertices[] = {
        // positions            // colors
        -0.5f, 0.0f, 0.0f,   1.0f,0.0f,0.0f,
         0.0f, 0.5f, 0.0f,   0.0f,1.0f,0.0f,
         0.5f, 0.0f, 0.0f,   0.0f,0.0f,1.0f
    };

    unsigned int indices[] = { 0, 1, 2 };



    engine::VAO vao;
    vao.Bind();

    engine::VBO vbo;
    vbo.SetData(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    engine::EBO ebo;
    ebo.SetData(indices, sizeof(indices), GL_STATIC_DRAW);


    engine::Shader shader("game/assets/shaders/basic.vert",
                  "game/assets/shaders/basic.frag");


    GLsizei stride = 6 * sizeof(float);              



    // Vertex attributes
    vao.AddAttribute(shader, "aPos", stride, 0);
    vao.AddAttribute(shader, "aColor", stride, 3*sizeof(float));


    vao.Unbind();
    vbo.Unbind();


    // Main loop
    while (!glfwWindowShouldClose(window)) {  
        // Clear screen
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our triangle
        shader.use();                          // use shader program
        vao.Bind();

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);



        // Swap buffers + process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
