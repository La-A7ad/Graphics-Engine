#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine/World.hpp"
#include "Engine/Entity.hpp"
#include "Engine/CameraComponent.hpp"
#include "Engine/MeshRendererComponent.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/ShaderLoader.hpp"
#include "Engine/TextureLoader.hpp"
#include "Engine/MeshLoader.hpp"
#include "Engine/TintedMaterial.hpp"

using namespace engine;

GLFWwindow* window = nullptr;
int width = 1280;
int height = 720;

void framebuffer_size_callback(GLFWwindow* w, int w_new, int h_new) {
    width = w_new;
    height = h_new;
}

bool InitWindow() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(width, height, "Graphics Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }
    
    return true;
}

void ProcessInput(Entity* cameraEntity, float deltaTime) {
    float speed = 5.0f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraEntity->position += cameraEntity->GetForward() * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraEntity->position -= cameraEntity->GetForward() * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraEntity->position -= cameraEntity->GetRight() * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraEntity->position += cameraEntity->GetRight() * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraEntity->position.y += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraEntity->position.y -= speed;
    }
}

// Create the cube mesh
Mesh* CreateCubeMesh() {
    std::vector<Vertex> vertices = {
        // Front
        {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},
        
        // Back
        {{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
        
        // Top
        {{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
        
        // Bottom
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
        
        // Right
        {{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
        
        // Left
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 1.0f}}
    };
    
    std::vector<unsigned int> indices = {
        0,  1,  2,   2,  3,  0,   // Front
        4,  5,  6,   6,  7,  4,   // Back
        8,  9, 10,  10, 11,  8,   // Top
        12, 13, 14,  14, 15, 12,  // Bottom
        16, 17, 18,  18, 19, 16,  // Right
        20, 21, 22,  22, 23, 20   // Left
    };
    
    return new Mesh(vertices, indices);
}

int main() {
    if (!InitWindow()) {
        return -1;
    }
    
    World world;
    Renderer renderer;
    if (!renderer.Init()) {
        std::cerr << "Failed to initialize renderer\n";
        return -1;
    }
    
    ShaderLoader::Instance().Load("basic", 
        "game/assets/shaders/basic.vert",
        "game/assets/shaders/basic.frag");
    
    Entity* camera = world.CreateEntity("Camera");
    camera->position = glm::vec3(0, 2, 5);
    auto* camComp = camera->AddComponent<CameraComponent>();
    camComp->cameraType = CameraComponent::PERSPECTIVE;
    camComp->fovY = glm::radians(60.0f);
    camComp->nearPlane = 0.1f;
    camComp->farPlane = 100.0f;
    
    // Create cube mesh using the new function
    Mesh* cubeMesh = CreateCubeMesh();
    
    Entity* cube = world.CreateEntity("Cube");
    cube->position = glm::vec3(0, 0, 0);
    auto* meshRenderer = cube->AddComponent<MeshRendererComponent>();
    
    auto* material = new TintedMaterial();
    material->shader = ShaderLoader::Instance().Get("basic");
    material->tint = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
    material->transparent = false;
    
    meshRenderer->material = material;
    meshRenderer->mesh = cubeMesh;  // Assign the created cube mesh
    
    float lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        ProcessInput(camera, deltaTime);
        
        cube->rotation.y += deltaTime * 0.5f;
        
        renderer.Resize(width, height);
        renderer.Render(&world, camComp);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete material;
    delete cubeMesh; // Clean up the created mesh
    
    ShaderLoader::Instance().Clear();
    TextureLoader::Instance().Clear();
    MeshLoader::Instance().Clear();
    
    glfwTerminate();
    return 0;
}
