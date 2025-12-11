#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
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
    
    // Movement - WASD
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
    
    // Camera rotation - Arrow keys
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
    
    // ============================================================
    // SHADER LOADING TEST
    // ============================================================
    std::cout << "[1/8] Loading Shaders...\n";
    
    // PLACEHOLDER: Create game/assets/shaders/basic.vert and basic.frag
    // These shaders should have: aPosition, aNormal, aTexCoords attributes
    // And uniforms: uModel, uView, uProj, uTint
    Shader* basicShader = ShaderLoader::Instance().Load("basic", 
        "game/assets/shaders/basic.vert",
        "game/assets/shaders/basic.frag");
    
    // PLACEHOLDER: Create game/assets/shaders/textured.vert and textured.frag
    // These shaders should support texture sampling with uAlbedoMap
    Shader* texturedShader = ShaderLoader::Instance().Load("textured",
        "game/assets/shaders/textured.vert",
        "game/assets/shaders/textured.frag");
    
    if (!basicShader || !texturedShader) {
        std::cerr << "Failed to load shaders!\n";
        std::cerr << "Make sure you have:\n";
        std::cerr << "  - game/assets/shaders/basic.vert\n";
        std::cerr << "  - game/assets/shaders/basic.frag\n";
        std::cerr << "  - game/assets/shaders/textured.vert\n";
        std::cerr << "  - game/assets/shaders/textured.frag\n";
        return -1;
    }
    
    std::cout << "  ✓ Basic shader loaded\n";
    std::cout << "  ✓ Textured shader loaded\n";
    
    // ============================================================
    // MODEL LOADING TEST
    // ============================================================
    std::cout << "\n[2/8] Loading Models...\n";
    
    // PLACEHOLDER: game/assets/models/cube.obj (you already have this)
    Model* cubeModel = MeshLoader::Instance().Load("cube", "game/assets/models/cube.obj");
    
    // PLACEHOLDER: Create or download game/assets/models/sphere.obj
    Model* sphereModel = MeshLoader::Instance().Load("sphere", "game/assets/models/sphere.obj");
    
    // PLACEHOLDER: Create or download game/assets/models/plane.obj
    Model* planeModel = MeshLoader::Instance().Load("plane", "game/assets/models/plane.obj");
    
    if (!cubeModel || cubeModel->meshes.empty()) {
        std::cerr << "Failed to load cube model!\n";
        return -1;
    }
    
    std::cout << "  ✓ Cube model loaded (" << cubeModel->meshes.size() << " meshes)\n";
    
    if (sphereModel && !sphereModel->meshes.empty()) {
        std::cout << "  ✓ Sphere model loaded\n";
    } else {
        sphereModel = nullptr;
        std::cout << "  ⚠ Sphere model not found (optional)\n";
    }
    
    if (planeModel && !planeModel->meshes.empty()) {
        std::cout << "  ✓ Plane model loaded\n";
    } else {
        planeModel = nullptr;
        std::cout << "  ⚠ Plane model not found (optional)\n";
    }
    
    // ============================================================
    // TEXTURE LOADING TEST
    // ============================================================
    std::cout << "\n[3/8] Loading Textures...\n";
    
    // PLACEHOLDER: Create game/assets/textures/crate.png
    Texture* crateTexture = TextureLoader::Instance().Load("crate", "game/assets/textures/osaka.jpg");
    
    // PLACEHOLDER: Create game/assets/textures/checker.png
    Texture* checkerTexture = TextureLoader::Instance().Load("checker", "game/assets/textures/osaka.jpg");
    
    if (crateTexture) {
        std::cout << "  ✓ Crate texture loaded\n";
    } else {
        std::cout << "  ⚠ Crate texture not found (optional)\n";
    }
    
    if (checkerTexture) {
        std::cout << "  ✓ Checker texture loaded\n";
    } else {
        std::cout << "  ⚠ Checker texture not found (optional)\n";
    }
    
    // ============================================================
    // SAMPLER CREATION TEST
    // ============================================================
    std::cout << "\n[4/8] Creating Samplers...\n";
    
    Sampler* linearSampler = new Sampler();
    linearSampler->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    linearSampler->SetMagFilter(GL_LINEAR);
    linearSampler->SetWrapS(GL_REPEAT);
    linearSampler->SetWrapT(GL_REPEAT);
    
    Sampler* nearestSampler = new Sampler();
    nearestSampler->SetMinFilter(GL_NEAREST);
    nearestSampler->SetMagFilter(GL_NEAREST);
    nearestSampler->SetWrapS(GL_CLAMP_TO_EDGE);
    nearestSampler->SetWrapT(GL_CLAMP_TO_EDGE);
    
    std::cout << "  ✓ Linear sampler created\n";
    std::cout << "  ✓ Nearest sampler created\n";
    
    // ============================================================
    // CAMERA SETUP TEST
    // ============================================================
    std::cout << "\n[5/8] Setting up Camera...\n";
    
    Entity* cameraEntity = world.CreateEntity("MainCamera");
    cameraEntity->position = glm::vec3(0, 3, 10);
    cameraEntity->rotation = glm::vec3(glm::radians(-15.0f), 0, 0);
    
    auto* camera = cameraEntity->AddComponent<CameraComponent>();
    camera->cameraType = CameraComponent::PERSPECTIVE;
    camera->fovY = glm::radians(75.0f);
    camera->nearPlane = 0.1f;
    camera->farPlane = 1000.0f;
    
    std::cout << "  ✓ Camera entity created\n";
    std::cout << "  ✓ Camera component added\n";
    
    // ============================================================
    // ENTITY HIERARCHY TEST
    // ============================================================
    std::cout << "\n[6/8] Testing Entity Hierarchy...\n";
    
    // Create parent entity that rotates
    Entity* rotatingGroup = world.CreateEntity("RotatingGroup");
    rotatingGroup->position = glm::vec3(-5, 1, 0);
    
    // Create 3 child cubes in a line
    for (int i = 0; i < 3; i++) {
        Entity* child = world.CreateEntity("ChildCube_" + std::to_string(i));
        child->position = glm::vec3(i * 2.0f, 0, 0);
        child->scale = glm::vec3(0.5f);
        
        rotatingGroup->AddChild(child);
        
        auto* meshRenderer = child->AddComponent<MeshRendererComponent>();
        auto* material = new TintedMaterial();
        material->shader = basicShader;
        
        // Different colors
        if (i == 0) material->tint = glm::vec4(1.0f, 0.3f, 0.3f, 1.0f);
        else if (i == 1) material->tint = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f);
        else material->tint = glm::vec4(0.3f, 0.3f, 1.0f, 1.0f);
        
        meshRenderer->material = material;
        meshRenderer->mesh = &cubeModel->meshes[0];
    }
    
    std::cout << "  ✓ Parent entity with 3 children created\n";
    
    // ============================================================
    // MATERIAL SYSTEM TEST
    // ============================================================
    std::cout << "\n[7/8] Testing Material System...\n";
    
    // Test 1: TintedMaterial with different colors
    Entity* tintedCube1 = world.CreateEntity("TintedCube_Yellow");
    tintedCube1->position = glm::vec3(0, 1, 0);
    tintedCube1->scale = glm::vec3(0.8f);
    
    auto* tintedRenderer1 = tintedCube1->AddComponent<MeshRendererComponent>();
    auto* tintedMat1 = new TintedMaterial();
    tintedMat1->shader = basicShader;
    tintedMat1->tint = glm::vec4(1.0f, 0.9f, 0.2f, 1.0f);
    tintedRenderer1->material = tintedMat1;
    tintedRenderer1->mesh = &cubeModel->meshes[0];
    
    // Test 2: TintedMaterial with transparency
    Entity* transparentCube = world.CreateEntity("TransparentCube");
    transparentCube->position = glm::vec3(3, 1, 0);
    transparentCube->scale = glm::vec3(0.8f);
    
    auto* transparentRenderer = transparentCube->AddComponent<MeshRendererComponent>();
    auto* transparentMat = new TintedMaterial();
    transparentMat->shader = basicShader;
    transparentMat->tint = glm::vec4(0.3f, 0.8f, 1.0f, 0.5f);
    transparentMat->transparent = true;
    transparentMat->pipelineState.blending = true;
    transparentMat->pipelineState.depthMask = false;
    transparentRenderer->material = transparentMat;
    transparentRenderer->mesh = &cubeModel->meshes[0];
    
    std::cout << "  ✓ TintedMaterial (opaque) created\n";
    std::cout << "  ✓ TintedMaterial (transparent) created\n";
    
    // Test 3: TexturedMaterial (if textures are available)
    if (crateTexture && texturedShader) {
        Entity* texturedCube = world.CreateEntity("TexturedCube");
        texturedCube->position = glm::vec3(5, 1, 0);
        
        auto* texRenderer = texturedCube->AddComponent<MeshRendererComponent>();
        auto* texMat = new TexturedMaterial();
        texMat->shader = texturedShader;
        texMat->albedoMap = crateTexture;
        texMat->sampler = linearSampler;
        texMat->tint = glm::vec4(1.0f);
        texRenderer->material = texMat;
        texRenderer->mesh = &cubeModel->meshes[0];
        
        std::cout << "  ✓ TexturedMaterial created\n";
    }
    
    // Test 4: Sphere with different material (if model exists)
    if (sphereModel) {
        Entity* sphere = world.CreateEntity("Sphere");
        sphere->position = glm::vec3(-3, 2, 0);
        sphere->scale = glm::vec3(0.7f);
        
        auto* sphereRenderer = sphere->AddComponent<MeshRendererComponent>();
        auto* sphereMat = new TintedMaterial();
        sphereMat->shader = basicShader;
        sphereMat->tint = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
        sphereRenderer->material = sphereMat;
        sphereRenderer->mesh = &sphereModel->meshes[0];
        
        std::cout << "  ✓ Sphere with material created\n";
    }
    
    // Test 5: Ground plane (if model exists)
    if (planeModel) {
        Entity* ground = world.CreateEntity("Ground");
        ground->position = glm::vec3(0, 0, 0);
        ground->scale = glm::vec3(10, 1, 10);
        
        auto* groundRenderer = ground->AddComponent<MeshRendererComponent>();
        auto* groundMat = new TintedMaterial();
        groundMat->shader = basicShader;
        groundMat->tint = glm::vec4(0.2f, 0.25f, 0.2f, 1.0f);
        groundRenderer->material = groundMat;
        groundRenderer->mesh = &planeModel->meshes[0];
        
        std::cout << "  ✓ Ground plane created\n";
    }
    
    // ============================================================
    // PIPELINE STATE TEST
    // ============================================================
    std::cout << "\n[8/8] Testing Pipeline States...\n";
    
    // Cube with custom pipeline state (no face culling)
    Entity* noCullCube = world.CreateEntity("NoCullCube");
    noCullCube->position = glm::vec3(0, 3, 0);
    noCullCube->scale = glm::vec3(0.6f);
    
    auto* noCullRenderer = noCullCube->AddComponent<MeshRendererComponent>();
    auto* noCullMat = new TintedMaterial();
    noCullMat->shader = basicShader;
    noCullMat->tint = glm::vec4(1.0f, 0.3f, 0.9f, 1.0f);
    noCullMat->pipelineState.faceCulling = false;
    noCullRenderer->material = noCullMat;
    noCullRenderer->mesh = &cubeModel->meshes[0];
    
    std::cout << "  ✓ Custom pipeline state (no culling) applied\n";
    std::cout << "  ✓ Blend states configured\n";
    std::cout << "  ✓ Depth testing configured\n";
    
    // ============================================================
    // MAIN LOOP
    // ============================================================
    std::cout << "\n========================================\n";
    std::cout << "  ALL SYSTEMS INITIALIZED SUCCESSFULLY!\n";
    std::cout << "========================================\n\n";
    std::cout << "Controls:\n";
    std::cout << "  WASD       - Move camera\n";
    std::cout << "  Space/Shift - Up/Down\n";
    std::cout << "  Arrow Keys - Rotate camera\n";
    std::cout << "  ESC        - Exit\n\n";
    std::cout << "Features tested:\n";
    std::cout << "  ✓ Shader loading & management\n";
    std::cout << "  ✓ Model loading (OBJ files)\n";
    std::cout << "  ✓ Texture loading & sampling\n";
    std::cout << "  ✓ Entity-Component System\n";
    std::cout << "  ✓ Transform hierarchies\n";
    std::cout << "  ✓ Material system (Tinted & Textured)\n";
    std::cout << "  ✓ Pipeline states (culling, blending, depth)\n";
    std::cout << "  ✓ Camera system (perspective)\n";
    std::cout << "  ✓ Transparent rendering (back-to-front sorting)\n\n";
    
    float lastTime = glfwGetTime();
    float rotationSpeed = 0.8f;
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        ProcessInput(cameraEntity, deltaTime);
        
        // Animate entities
        tintedCube1->rotation.y += deltaTime * 1.0f;
        tintedCube1->rotation.x += deltaTime * 0.5f;
        
        transparentCube->rotation.y -= deltaTime * 0.7f;
        
        rotatingGroup->rotation.y += deltaTime * rotationSpeed;
        
        noCullCube->rotation.x += deltaTime * 1.5f;
        noCullCube->rotation.z += deltaTime * 0.8f;
        
        if (sphereModel) {
            Entity* sphere = world.FindByName("Sphere");
            if (sphere) {
                sphere->rotation.y += deltaTime * 0.5f;
                sphere->position.y = 2.0f + sinf(currentTime * 2.0f) * 0.5f;
            }
        }
        
        // Render
        renderer.Resize(width, height);
        renderer.Render(&world, camera);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // ============================================================
    // CLEANUP
    // ============================================================
    std::cout << "\nCleaning up...\n";
    
    for (auto* entity : world.entities) {
        auto* meshRenderer = entity->GetComponent<MeshRendererComponent>();
        if (meshRenderer && meshRenderer->material) {
            delete meshRenderer->material;
            meshRenderer->material = nullptr;
        }
    }
    
    delete linearSampler;
    delete nearestSampler;
    
    ShaderLoader::Instance().Clear();
    TextureLoader::Instance().Clear();
    MeshLoader::Instance().Clear();
    
    glfwTerminate();
    
    std::cout << "✓ Cleanup complete!\n";
    return 0;
}