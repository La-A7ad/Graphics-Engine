#include <glad/glad.h>      // MUST be first OpenGL header
#include <GLFW/glfw3.h>

#include <iostream>

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

    //We are about to test a triangle

    float triangleVertices[] = {
        //   x       y      z
        -0.5f, -0.5f, 0.0f,   // left
         0.5f, -0.5f, 0.0f,   // right
         0.0f,  0.5f, 0.0f    // top
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //Generates vertex array
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO); //(GLsizei size, GLuint* buffers)
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW); //uses buffer call on the data we want to use and how we wanna draw it

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);//index, size, datatype, normalised,stride, offset pointer
    glEnableVertexAttribArray(0);

    const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    //for OpenGL to use this shader it has to compile it at runtime from its source code, for that we initialise a vertex object
    GLuint VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(VertexShader, 1, &vertexShaderSource, NULL); //
    glCompileShader(VertexShader);

    int success;
    GLchar infoLog[512]; //used in in shaderinfolog
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
        std::cout<<"Vertex Shader Compilation failed";
    }

    const char *FragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor; \n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

    GLuint FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);

    glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);

    GLuint ShaderProgram;
    ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glGetShaderiv(ShaderProgram, GL_COMPILE_STATUS, &success);

    glUseProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader); //After we linked the shader objects to the program we no longer need them 

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear screen
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
