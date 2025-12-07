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
        -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // bottom-left (red)
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // bottom-right (green)
         0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // top (blue)
    };

    unsigned int indices[] = { 0, 1, 2 };

    // Create shader first
    engine::Shader shader("game/assets/shaders/basic.vert",
                          "game/assets/shaders/basic.frag");

    // Debug: Check if attributes were found
    std::cout << "Checking shader attributes...\n";
    const auto* posAttr = shader.getAttrib("aPos");
    const auto* colorAttr = shader.getAttrib("aColor");
    
    if (posAttr) {
        std::cout << "aPos found - location: " << posAttr->location 
                  << ", components: " << posAttr->components << "\n";
    } else {
        std::cout << "ERROR: aPos not found!\n";
    }
    
    if (colorAttr) {
        std::cout << "aColor found - location: " << colorAttr->location 
                  << ", components: " << colorAttr->components << "\n";
    } else {
        std::cout << "ERROR: aColor not found!\n";
    }

    // Create and bind VAO
    engine::VAO vao;
    vao.Bind();

    // Create VBO and upload data
    engine::VBO vbo;
    vbo.SetData(Vertices, sizeof(Vertices), GL_STATIC_DRAW);

    // Create EBO and upload data
    engine::EBO ebo;
    ebo.SetData(indices, sizeof(indices), GL_STATIC_DRAW);

    // IMPORTANT: Make sure VBO is bound before setting up attributes
    vbo.Bind();

    GLsizei stride = 6 * sizeof(float);

    // Set up vertex attributes
    std::cout << "Setting up vertex attributes...\n";
    vao.AddAttribute(shader, "aPos", stride, 0);
    vao.AddAttribute(shader, "aColor", stride, 3 * sizeof(float));
    std::cout << "Vertex attributes set up complete.\n";

    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << "\n";
    }

    // Unbind (optional, but good practice)
    vao.Unbind();
    vbo.Unbind();

    std::cout << "Starting render loop...\n";

    // Main loop
    while (!glfwWindowShouldClose(window)) {  
        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark gray background
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our triangle
        shader.use();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Check for errors in render loop (only once)
        static bool errorChecked = false;
        if (!errorChecked) {
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cout << "Render OpenGL error: " << err << "\n";
            }
            errorChecked = true;
        }

        // Swap buffers + process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}