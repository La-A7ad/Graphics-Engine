#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Engine/Engine.hpp"
#include "Materials/PS1Material.hpp"
#include <memory>
#include <string>
#include <glm/glm.hpp>

#define LOG(msg) std::cout << msg << std::endl

int main() {
    engine::Renderer renderer;
    if (!renderer.Init()) {
        std::cerr << "Failed to initialize renderer.\n";
        return -1;
    }
    
    GLFWwindow* window = renderer.GetWindow();
    if (!window) {
        std::cerr << "Renderer returned a null window.\n";
        return -1;
    }

    engine::World world;
    
    // Camera
    engine::Entity* cameraEntity = world.CreateEntity("MainCamera");
    if (!cameraEntity) {
        std::cerr << "Failed to create camera entity.\n";
        return -1;
    }
    
    auto* camera = cameraEntity->AddComponent<engine::CameraComponent>();
    if (!camera) {
        std::cerr << "Failed to add CameraComponent.\n";
        return -1;
    }
    
    cameraEntity->position = glm::vec3(0.0f, 0.6f, 10.0f);
    cameraEntity->rotation = glm::vec3(0.0f);
    cameraEntity->scale    = glm::vec3(1.0f);

    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    if (fbWidth > 0 && fbHeight > 0) {
        camera->aspectRatio = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
    }
    
    // Load Cat glTF
    const std::string modelPath = "game/assets/models/Cat/scene.gltf";
    engine::Model* catModel = engine::MeshLoader::Instance().Load("cat", modelPath);
    if (!catModel || catModel->meshes.empty()) {
        std::cerr << "Failed to load Cat model or it has no meshes. Path: " << modelPath << "\n";
        return -1;
    }
    
    // Load Cat baseColor texture
    const std::string texPath = "game/assets/models/Cat/textures/Material.002_baseColor.png";
    engine::Texture* catTex = engine::TextureLoader::Instance().Load("cat_albedo", texPath);
    if (!catTex) {
        std::cerr << "Failed to load Cat texture. Path: " << texPath << "\n";
        return -1;
    }
    
    // Create entity
    engine::Entity* catEntity = world.CreateEntity("Cat");
    if (!catEntity) {
        std::cerr << "Failed to create Cat entity.\n";
        return -1;
    }
    
    catEntity->position = glm::vec3(0.0f, -0.5f, 0.0f);
    catEntity->rotation = glm::vec3(0.0f);
    catEntity->scale    = glm::vec3(1.0f);

    auto* mr = catEntity->AddComponent<engine::MeshRendererComponent>();
    if (!mr) {
        std::cerr << "Failed to add MeshRendererComponent to Cat.\n";
        return -1;
    }
    
    mr->mesh = &catModel->meshes[0];
    
    // === PS1 MATERIAL ===
    auto mat = std::make_unique<PS1Material>();
    
    mat->shader = std::make_shared<engine::Shader>(
        "game/assets/shaders/psx_unlit.vert",
        "game/assets/shaders/psx_unlit.frag"
    );
    
    mat->albedoMap = catTex;
    mat->tint = glm::vec4(1.0f);
    
    // Choose PS1 style:
    mat->SetAuthenticPS1();      // Maximum wobble & banding
    // mat->SetPS1Inspired();    // Subtle/playable
    // mat->SetAffineOnly();     // Just texture warping
    
    mr->material = std::move(mat);
    
    LOG("✓ PS1 Cat loaded");
    LOG("Entering main loop...");
    
    float lastTime = static_cast<float>(glfwGetTime());
    float angle = 0.0f;
    
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        angle += deltaTime * 0.7f;
        catEntity->rotation.y = angle;
        
        renderer.Render(&world, camera);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    engine::ShaderLoader::Instance().Clear();
    engine::MeshLoader::Instance().Clear();
    engine::TextureLoader::Instance().Clear();
    
    LOG("Exiting...");
    return 0;
}