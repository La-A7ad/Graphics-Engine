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
#include "Engine/TexturedMaterial.hpp"
#include "Engine/Rendering/Geometry/Model/Model.hpp"

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
    
    window = glfwCreateWindow(width, height, "Graphics Engine - Fixed", nullptr, nullptr);
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
    float rotSpeed = 2.0f * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // Movement
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
    
    // Camera rotation
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraEntity->rotation.y += rotSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraEntity->rotation.y -= rotSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraEntity->rotation.x += rotSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraEntity->rotation.x -= rotSpeed;
    }
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
    
    // ============================================================
    // FIXED: Load shader properly
    // ============================================================
    ShaderLoader::Instance().Load("basic", 
        "game/assets/shaders/basic.vert",
        "game/assets/shaders/basic.frag");
    
    Shader* basicShader = ShaderLoader::Instance().Get("basic");
    if (!basicShader) {
        std::cerr << "Failed to load basic shader\n";
        return -1;
    }
    
    // ============================================================
    // FIXED: Use MeshLoader to load OBJ file instead of manual creation
    // ============================================================
    Model* cubeModel = MeshLoader::Instance().Load("cube", "game/assets/models/cube.obj");
    if (!cubeModel) {
        std::cerr << "Failed to load cube model\n";
        return -1;
    }
    
    // ============================================================
    // Setup Camera
    // ============================================================
    Entity* camera = world.CreateEntity("Camera");
    camera->position = glm::vec3(0, 2, 5);
    auto* camComp = camera->AddComponent<CameraComponent>();
    camComp->cameraType = CameraComponent::PERSPECTIVE;
    camComp->fovY = glm::radians(60.0f);
    camComp->nearPlane = 0.1f;
    camComp->farPlane = 100.0f;
    
    // ============================================================
    // FIXED: Use entity hierarchy (parent-child relationship)
    // Create a parent entity that rotates, with child cubes
    // ============================================================
    
    // Parent entity (invisible, just for grouping)
    Entity* cubeGroup = world.CreateEntity("CubeGroup");
    cubeGroup->position = glm::vec3(0, 0, 0);
    
    // Create multiple cubes as children using the loaded model
    for (int i = 0; i < 3; i++) {
        Entity* cube = world.CreateEntity("Cube_" + std::to_string(i));
        
        // Position relative to parent
        cube->position = glm::vec3(i * 2.0f - 2.0f, 0, 0);
        cube->scale = glm::vec3(0.5f);
        
        // FIXED: Use the entity hierarchy system
        cubeGroup->AddChild(cube);
        
        // Add mesh renderer with loaded model
        auto* meshRenderer = cube->AddComponent<MeshRendererComponent>();
        
        // Create material with different colors
        auto* material = new TintedMaterial();
        material->shader = basicShader;
        
        // Different color for each cube
        if (i == 0) material->tint = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f); // Red
        else if (i == 1) material->tint = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f); // Green
        else material->tint = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f); // Blue
        
        material->transparent = false;
        
        meshRenderer->material = material;
        // FIXED: Use the first mesh from the loaded model
        meshRenderer->mesh = &cubeModel->meshes[0];
    }
    
    // ============================================================
    // Create a single rotating cube (not in hierarchy)
    // ============================================================
    Entity* singleCube = world.CreateEntity("SingleCube");
    singleCube->position = glm::vec3(0, 2, 0);
    singleCube->scale = glm::vec3(0.8f);
    
    auto* singleRenderer = singleCube->AddComponent<MeshRendererComponent>();
    
    auto* singleMaterial = new TintedMaterial();
    singleMaterial->shader = basicShader;
    singleMaterial->tint = glm::vec4(1.0f, 0.8f, 0.2f, 1.0f); // Yellow
    singleMaterial->transparent = false;
    
    singleRenderer->material = singleMaterial;
    singleRenderer->mesh = &cubeModel->meshes[0];
    
    // ============================================================
    // Optional: Demonstrate TexturedMaterial setup
    // (You would need to add texture files to use this)
    // ============================================================
    /*
    // Example of how to use TexturedMaterial (commented out since no texture files exist):
    
    Texture* albedo = TextureLoader::Instance().Load("crate", "game/assets/textures/crate.png");
    
    Entity* texturedCube = world.CreateEntity("TexturedCube");
    texturedCube->position = glm::vec3(3, 0, 0);
    
    auto* texRenderer = texturedCube->AddComponent<MeshRendererComponent>();
    
    auto* texMaterial = new TexturedMaterial();
    texMaterial->shader = basicShader;
    texMaterial->albedoMap = albedo;
    texMaterial->tint = glm::vec4(1.0f);
    texMaterial->transparent = false;
    
    texRenderer->material = texMaterial;
    texRenderer->mesh = &cubeModel->meshes[0];
    */
    
    // ============================================================
    // Main Loop
    // ============================================================
    float lastTime = glfwGetTime();
    
    std::cout << "\n=== Engine Fixed! ===\n";
    std::cout << "Using MeshLoader to load cube.obj\n";
    std::cout << "Demonstrating entity hierarchy (parent rotates, children follow)\n";
    std::cout << "Removed useless Transform.cpp\n";
    std::cout << "\nControls:\n";
    std::cout << "  WASD - Move camera\n";
    std::cout << "  Space/Shift - Up/Down\n";
    std::cout << "  Arrow Keys - Rotate camera\n";
    std::cout << "  ESC - Exit\n\n";
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        ProcessInput(camera, deltaTime);
        
        // Rotate the single cube on its own
        singleCube->rotation.y += deltaTime * 1.0f;
        singleCube->rotation.x += deltaTime * 0.5f;
        
        // Rotate the parent group - all children will rotate with it!
        // This demonstrates the entity hierarchy system
        cubeGroup->rotation.y += deltaTime * 0.5f;
        
        renderer.Resize(width, height);
        renderer.Render(&world, camComp);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // ============================================================
    // Cleanup
    // ============================================================
    // Clean up materials
    for (auto* entity : world.entities) {
        auto* meshRenderer = entity->GetComponent<MeshRendererComponent>();
        if (meshRenderer && meshRenderer->material) {
            delete meshRenderer->material;
            meshRenderer->material = nullptr;
        }
    }
    
    // Note: Don't delete the meshes - they're owned by the Model
    // which is managed by MeshLoader
    
    ShaderLoader::Instance().Clear();
    TextureLoader::Instance().Clear();
    MeshLoader::Instance().Clear();
    
    glfwTerminate();
    return 0;
}