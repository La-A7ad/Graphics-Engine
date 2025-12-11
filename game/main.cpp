#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <glm/gtc/constants.hpp>

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
#include "Engine/Model.hpp"
#include "Engine/Sampler.hpp"

using namespace engine;

GLFWwindow* window = nullptr;
int width = 1600;
int height = 900;

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
    
    window = glfwCreateWindow(width, height, "Graphics Engine - Full Test", nullptr, nullptr);
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
    
    std::cout << "\n========================================\n";
    std::cout << "  GRAPHICS ENGINE - COMPREHENSIVE TEST\n";
    std::cout << "========================================\n\n";
    
    // Load shaders (now with error handling)
    std::cout << "[1/8] Loading Shaders...\n";
    
    Shader* basicShader = ShaderLoader::Instance().Load("basic", 
        "game/assets/shaders/basic.vert",
        "game/assets/shaders/basic.frag");
    
    Shader* texturedShader = ShaderLoader::Instance().Load("textured",
        "game/assets/shaders/textured.vert",
        "game/assets/shaders/textured.frag");
    
    if (!basicShader || !texturedShader) {
        std::cerr << "Critical shaders missing! Exiting.\n";
        return -1;
    }
    
    // Load models
    std::cout << "\n[2/8] Loading Models...\n";
    Model* cubeModel = MeshLoader::Instance().Load("cube", "game/assets/models/cube.obj");
    
    if (!cubeModel || cubeModel->meshes.empty()) {
        std::cerr << "Failed to load cube model!\n";
        return -1;
    }
    std::cout << "  ✓ Cube model loaded\n";
    
    // Load textures
    std::cout << "\n[3/8] Loading Textures...\n";
    Texture* crateTexture = TextureLoader::Instance().Load("crate", "game/assets/textures/osaka.jpg");
    if (crateTexture) {
        std::cout << "  ✓ Crate texture loaded\n";
    }
    
    // Create shared samplers (smart pointers for automatic cleanup)
    std::cout << "\n[4/8] Creating Samplers...\n";
    
    auto linearSampler = std::make_shared<Sampler>();
    linearSampler->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    linearSampler->SetMagFilter(GL_LINEAR);
    linearSampler->SetWrapS(GL_REPEAT);
    linearSampler->SetWrapT(GL_REPEAT);
    std::cout << "  ✓ Linear sampler created\n";
    
    // Setup camera
    std::cout << "\n[5/8] Setting up Camera...\n";
    Entity* cameraEntity = world.CreateEntity("MainCamera");
    cameraEntity->position = glm::vec3(0, 3, 10);
    cameraEntity->rotation = glm::vec3(glm::radians(-15.0f), 0, 0);
    
    auto* camera = cameraEntity->AddComponent<CameraComponent>();
    camera->cameraType = CameraComponent::PERSPECTIVE;
    camera->fovY = glm::radians(75.0f);
    camera->nearPlane = 0.1f;
    camera->farPlane = 1000.0f;
    std::cout << "  ✓ Camera created\n";
    
    // Test entity hierarchy
    std::cout << "\n[6/8] Testing Entity Hierarchy...\n";
    Entity* rotatingGroup = world.CreateEntity("RotatingGroup");
    rotatingGroup->position = glm::vec3(-5, 1, 0);
    
    for (int i = 0; i < 3; i++) {
        Entity* child = world.CreateEntity("ChildCube_" + std::to_string(i));
        child->position = glm::vec3(i * 2.0f, 0, 0);
        child->scale = glm::vec3(0.5f);
        
        rotatingGroup->AddChild(child);
        
        auto* meshRenderer = child->AddComponent<MeshRendererComponent>();
        
        // FIXED: Use unique_ptr for automatic memory management
        auto material = std::make_unique<TintedMaterial>();
        material->shader = basicShader;
        
        if (i == 0) material->tint = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
        else if (i == 1) material->tint = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);
        else material->tint = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f);
        
        meshRenderer->material = std::move(material);  // Transfer ownership
        meshRenderer->mesh = &cubeModel->meshes[0];
    }
    std::cout << "  ✓ Parent entity with 3 children created\n";
    
    // Test materials
    std::cout << "\n[7/8] Testing Material System...\n";
    
    // Tinted cube
    Entity* tintedCube1 = world.CreateEntity("TintedCube_Yellow");
    tintedCube1->position = glm::vec3(0, 1, 0);
    tintedCube1->scale = glm::vec3(0.8f);
    
    auto* tintedRenderer1 = tintedCube1->AddComponent<MeshRendererComponent>();
    auto tintedMat1 = std::make_unique<TintedMaterial>();
    tintedMat1->shader = basicShader;
    tintedMat1->tint = glm::vec4(1.0f, 0.9f, 0.2f, 1.0f);
    tintedRenderer1->material = std::move(tintedMat1);
    tintedRenderer1->mesh = &cubeModel->meshes[0];
    
    // Transparent cube
    Entity* transparentCube = world.CreateEntity("TransparentCube");
    transparentCube->position = glm::vec3(3, 1, 0);
    transparentCube->scale = glm::vec3(0.8f);
    
    auto* transparentRenderer = transparentCube->AddComponent<MeshRendererComponent>();
    auto transparentMat = std::make_unique<TintedMaterial>();
    transparentMat->shader = basicShader;
    transparentMat->tint = glm::vec4(0.3f, 0.8f, 1.0f, 0.5f);
    transparentMat->transparent = true;
    transparentMat->pipelineState.blending = true;
    transparentMat->pipelineState.depthMask = false;
    transparentRenderer->material = std::move(transparentMat);
    transparentRenderer->mesh = &cubeModel->meshes[0];
    
    std::cout << "  ✓ TintedMaterial (opaque) created\n";
    std::cout << "  ✓ TintedMaterial (transparent) created\n";
    
    // Textured cube
    if (crateTexture && texturedShader) {
        Entity* texturedCube = world.CreateEntity("TexturedCube");
        texturedCube->position = glm::vec3(5, 1, 0);
        
        auto* texRenderer = texturedCube->AddComponent<MeshRendererComponent>();
        auto texMat = std::make_unique<TexturedMaterial>();
        texMat->shader = texturedShader;
        texMat->albedoMap = crateTexture;
        texMat->sampler = linearSampler;  // shared_ptr
        texMat->tint = glm::vec4(1.0f);
        texRenderer->material = std::move(texMat);
        texRenderer->mesh = &cubeModel->meshes[0];
        
        std::cout << "  ✓ TexturedMaterial created\n";
    }
    
    // Test pipeline states
    std::cout << "\n[8/8] Testing Pipeline States...\n";
    Entity* noCullCube = world.CreateEntity("NoCullCube");
    noCullCube->position = glm::vec3(0, 3, 0);
    noCullCube->scale = glm::vec3(0.6f);
    
    auto* noCullRenderer = noCullCube->AddComponent<MeshRendererComponent>();
    auto noCullMat = std::make_unique<TintedMaterial>();
    noCullMat->shader = basicShader;
    noCullMat->tint = glm::vec4(1.0f, 0.3f, 0.9f, 1.0f);
    noCullMat->pipelineState.faceCulling = false;
    noCullRenderer->material = std::move(noCullMat);
    noCullRenderer->mesh = &cubeModel->meshes[0];
    
    std::cout << "  ✓ Custom pipeline state applied\n";
    
    std::cout << "\n========================================\n";
    std::cout << "  ALL SYSTEMS INITIALIZED!\n";
    std::cout << "========================================\n\n";
    
    float lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        ProcessInput(cameraEntity, deltaTime);
        
        // Animate
        tintedCube1->rotation.y += deltaTime * 1.0f;
        transparentCube->rotation.y -= deltaTime * 0.7f;
        rotatingGroup->rotation.y += deltaTime * 0.8f;
        noCullCube->rotation.x += deltaTime * 1.5f;
        
        // Render
        renderer.Resize(width, height);
        renderer.Render(&world, camera);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // CLEANUP IS NOW AUTOMATIC!
    // - unique_ptr<Material> in components
    // - shared_ptr<Sampler> reference counted
    // - Loaders clean up on destruction
    
    std::cout << "\nCleaning up...\n";
    ShaderLoader::Instance().Clear();
    TextureLoader::Instance().Clear();
    MeshLoader::Instance().Clear();
    
    glfwTerminate();
    std::cout << "✓ Cleanup complete!\n";
    return 0;
}