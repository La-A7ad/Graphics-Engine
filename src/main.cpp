#include <glad/glad.h> //must be included before glfw
#include <GLFW/glfw3.h>
#include <iostream> //for debugging

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0, width, height); //resizes window so the rendered image doesnt stretch or get clipped

}

int main() {

    if (!glfwInit()) { //If this fails we wont have a window to deal with
        std::cerr << "Failed to initialise GLFW\n";

        return -1;

    }

    //OpenGL 3.3 babyyyyy, none of that fixed pipeline pre-Iraq war B.S

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Core Profile: removes all legacy functions allowing only shaders, VAO/VBO/EBO, Uniforms and modern pipelining

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Window Context:
    GLFWwindow* window = glfwCreateWindow(1920, 1080,"yeaah", NULL);


    glfwMakeContextCurrent(window); //activates context to the window we are working with

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))











    





    return 0;
}