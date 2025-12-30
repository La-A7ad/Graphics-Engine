#include "Engine/Engine.hpp"
#include "CameraController.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // ═══════════════════════════════════════════════════════════════
    // INITIALIZE RENDERER
    // ═══════════════════════════════════════════════════════════════
    engine::Renderer renderer;
    if (!renderer.Init()) {
        std::cerr << "Failed to initialize renderer\n";
        return -1;
    }
    
    // ═══════════════════════════════════════════════════════════════
    // INITIALIZE INPUT SYSTEM
    // ═══════════════════════════════════════════════════════════════
    auto& input = engine::InputManager::Instance();
    input.Init(renderer.GetWindow());
    
    // ═══════════════════════════════════════════════════════════════
    // LOAD SCENE FROM FILE
    // ═══════════════════════════════════════════════════════════════
    auto world = engine::SceneLoader::LoadScene("assets/scenes/example_scene.json");
    if (!world) {
        std::cerr << "Failed to load scene\n";
        return -1;
    }
    
    // ═══════════════════════════════════════════════════════════════
    // FIND CAMERA
    // ═══════════════════════════════════════════════════════════════
    engine::Entity* cameraEntity = world->FindByName("Camera");
    engine::CameraComponent* camera = nullptr;
    
    if (cameraEntity) {
        camera = cameraEntity->GetComponent<engine::CameraComponent>();
    }
    
    if (!camera) {
        std::cerr << "ERROR: No camera found in scene\n";
        return -1;
    }
    
    // ═══════════════════════════════════════════════════════════════
    // CREATE CAMERA CONTROLLER
    // ═══════════════════════════════════════════════════════════════
    CameraController cameraController(cameraEntity);
    
    // ═══════════════════════════════════════════════════════════════
    // GAME LOOP
    // ═══════════════════════════════════════════════════════════════
    std::cout << "\n";
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "       GAME RUNNING\n";
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "Controls:\n";
    std::cout << "  WASD      - Move camera\n";
    std::cout << "  Q/E       - Move up/down\n";
    std::cout << "  Right Mouse - Look around\n";
    std::cout << "  Shift     - Sprint\n";
    std::cout << "  ESC       - Exit\n";
    std::cout << "═══════════════════════════════════════\n\n";
    
    float lastFrame = 0.0f;
    
    while (!glfwWindowShouldClose(renderer.GetWindow())) {
        // Calculate delta time
        float currentFrame = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Poll input events
        glfwPollEvents();
        
        // Update input system
        input.Update();
        
        // Exit on ESC
        if (input.IsKeyPressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(renderer.GetWindow(), true);
        }
        
        // Update camera controller
        cameraController.Update(deltaTime);
        
        // ═══════════════════════════════════════════════════════════
        // UPDATE GAME LOGIC HERE
        // ═══════════════════════════════════════════════════════════
        
        // Example: Rotate an object
        engine::Entity* floatingCube = world->FindByName("Floating Cube");
        if (floatingCube) {
            floatingCube->rotation.y += 0.5f * deltaTime;
            floatingCube->rotation.x += 0.3f * deltaTime;
        }
        
        // ═══════════════════════════════════════════════════════════
        // RENDER
        // ═══════════════════════════════════════════════════════════
        renderer.Render(world.get(), camera);
        
        // Swap buffers
        glfwSwapBuffers(renderer.GetWindow());
    }
    
    // ═══════════════════════════════════════════════════════════════
    // CLEANUP
    // ═══════════════════════════════════════════════════════════════
    std::cout << "\n✓ Shutting down gracefully\n";
    
    // Clear asset loaders
    engine::ShaderLoader::Instance().Clear();
    engine::TextureLoader::Instance().Clear();
    engine::MeshLoader::Instance().Clear();
    
    return 0;
}