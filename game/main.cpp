#include "Engine/Engine.hpp"
#include "CameraController.hpp"
#include "Materials/PS1Material.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

void ConvertToPS1Materials(engine::World* world) {
    std::cout << "\n--- Converting to PS1 Materials ---\n";
    
    // Get the PSX shader
    engine::Shader* psxShader = engine::ShaderLoader::Instance().Get("psx");
    if (!psxShader) {
        std::cerr << "ERROR: PSX shader not found! Cannot convert to PS1 materials.\n";
        return;
    }
    
    // Iterate through all entities
    for (auto* entity : world->entities) {
        auto* renderer = entity->GetComponent<engine::MeshRendererComponent>();
        if (!renderer || !renderer->material) {
            continue;
        }
        
        // Create new PS1Material
        auto ps1Mat = std::make_unique<PS1Material>();
        
        // Copy shader reference
        ps1Mat->shader = std::shared_ptr<engine::Shader>(psxShader, [](engine::Shader*) {
            // Empty deleter - ShaderLoader owns the shader
        });
        
        // Copy properties from old material based on its type
        if (auto* oldTexMat = dynamic_cast<engine::TexturedMaterial*>(renderer->material.get())) {
            // Textured material - copy textures and tint
            ps1Mat->albedoMap = oldTexMat->albedoMap;
            ps1Mat->specularMap = oldTexMat->specularMap;
            ps1Mat->normalMap = oldTexMat->normalMap;
            ps1Mat->emissiveMap = oldTexMat->emissiveMap;
            ps1Mat->tint = oldTexMat->tint;
        } else if (auto* oldTintedMat = dynamic_cast<engine::TintedMaterial*>(renderer->material.get())) {
            // Tinted material - copy just the tint
            ps1Mat->tint = oldTintedMat->tint;
        } else {
            // Unknown material type - use white tint
            ps1Mat->tint = glm::vec4(1.0f);
        }
        
        // Copy pipeline state
        ps1Mat->pipelineState = renderer->material->pipelineState;
        
        // Apply PS1 effect preset
        ps1Mat->SetAuthenticPS1();  // Subtle, playable preset
        
        // Replace the material
        renderer->material = std::move(ps1Mat);
        
        std::cout << "  ✓ Converted " << entity->name << " to PS1Material\n";
    }
    
    std::cout << "--- PS1 Conversion Complete ---\n\n";
}

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
    auto world = engine::SceneLoader::LoadScene("game/assets/scenes/example_scene.json");
    if (!world) {
        std::cerr << "Failed to load scene\n";
        return -1;
    }
    
    // ═══════════════════════════════════════════════════════════════
    // CONVERT ALL MATERIALS TO PS1 STYLE
    // ═══════════════════════════════════════════════════════════════
    ConvertToPS1Materials(world.get());
    
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
    std::cout << "   PS1-STYLE RENDERER RUNNING\n";
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "Controls:\n";
    std::cout << "  WASD      - Move camera\n";
    std::cout << "  Q/E       - Move up/down\n";
    std::cout << "  Right Mouse - Look around\n";
    std::cout << "  Shift     - Sprint\n";
    std::cout << "  ESC       - Exit\n";
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "PS1 Effects Active:\n";
    std::cout << "  • Vertex snapping (wobble)\n";
    std::cout << "  • Affine texture mapping\n";
    std::cout << "  • Color depth reduction\n";
    std::cout << "  • Ordered dithering\n";
    std::cout << "  • Distance fog\n";
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