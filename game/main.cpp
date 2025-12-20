#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Engine/Engine.hpp"
#include <memory>
#include <string>

#include <glm/glm.hpp>

#define LOG(msg) std::cout << msg << std::endl

int main() {
    // Create renderer + init OpenGL context / window
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

    //Create world
    engine::World world;

    //Create camera entity
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

    // Position the camera a bit back on +Z
    cameraEntity->position = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraEntity->rotation = glm::vec3(0.0f);
    cameraEntity->scale    = glm::vec3(1.0f);

    // Set aspect ratio from framebuffer size
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    if (fbWidth > 0 && fbHeight > 0) {
        camera->aspectRatio = static_cast<float>(fbWidth) / static_cast<float>(fbHeight);
    }

    //Load a model via MeshLoader (lifetime managed there)
    std::string modelPath = "game/assets/models/cube.obj";
    engine::Model* cubeModel = engine::MeshLoader::Instance().Load("cube", modelPath);
    if (!cubeModel || cubeModel->meshes.empty()) {
        std::cerr << "Failed to load cube model or it has no meshes. Path: "
                  << modelPath << "\n";
        return -1;
    }

    //Create a tinted cube
    engine::Entity* tintedCube = world.CreateEntity("TintedCube");
    if (!tintedCube) {
        std::cerr << "Failed to create tinted cube entity.\n";
        return -1;
    }

    tintedCube->position = glm::vec3(2.0f, 0.0f, 0.0f);
    tintedCube->rotation = glm::vec3(0.0f);
    tintedCube->scale    = glm::vec3(1.0f);

    auto* tintedMR = tintedCube->AddComponent<engine::MeshRendererComponent>();
    if (!tintedMR) {
        std::cerr << "Failed to add MeshRendererComponent to tinted cube.\n";
        return -1;
    }

    // Point to the first mesh in the model (Mesh is non-owning)
    tintedMR->mesh = &cubeModel->meshes[0];

    // Create a tinted material with the basic shader
    auto tintedMat = std::make_unique<engine::TintedMaterial>();
    tintedMat->shader = std::make_shared<engine::Shader>(
        "game/assets/shaders/basic.vert",
        "game/assets/shaders/basic.frag"
    );
    // Bright orange so it's clearly visible
    tintedMat->tint = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);

    tintedMR->material = std::move(tintedMat);

    // Textured cube using TextureLoader + TexturedMaterial
    std::string texPath = "game/assets/textures/osaka.jpg";
    engine::Texture* albedoTex =
        engine::TextureLoader::Instance().Load("osaka", texPath);

    engine::Entity* texturedCube = nullptr;

    if (albedoTex) {
        texturedCube = world.CreateEntity("TexturedCube");
        if (texturedCube) {
            texturedCube->position = glm::vec3(0.0f, 0.0f, 0.0f);
            texturedCube->rotation = glm::vec3(0.0f);
            texturedCube->scale    = glm::vec3(1.0f);

            auto* texMR = texturedCube->AddComponent<engine::MeshRendererComponent>();
            if (texMR) {
                texMR->mesh = &cubeModel->meshes[0];

                auto texturedMat = std::make_unique<engine::TexturedMaterial>();
                texturedMat->shader = std::make_shared<engine::Shader>(
                    "game/assets/shaders/textured.vert",
                    "game/assets/shaders/textured.frag"
                );

                texturedMat->albedoMap = albedoTex;
                texturedMat->sampler   = std::make_shared<engine::Sampler>();
                texturedMat->tint      = glm::vec4(1.0f); // no color change

                texMR->material = std::move(texturedMat);
            }
        }
    } else {
        LOG("Warning: Failed to load texture " << texPath
            << ". Textured cube will be skipped.");
    }

    LOG("Entering main loop...");
    int frames = 0;

    // Rotation state
    float lastTime = glfwGetTime();
    float angle = 0.0f;

    //  Main loop
    while (!glfwWindowShouldClose(window)) {
        // ESC to quit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // Time & rotation
        float currentTime = glfwGetTime();
        float deltaTime   = currentTime - lastTime;
        lastTime = currentTime;

        angle += deltaTime * 1.0f; // 1 rad/s around Y axis

        tintedCube->rotation.y = angle;
        if (texturedCube) {
            texturedCube->rotation.y = angle;
            texturedCube->rotation.x = angle;
        }

        // Render the world from the main camera
        renderer.Render(&world, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        frames++;
        if (frames == 1) {
            LOG("First frame rendered successfully.");
        }
    }

    // Cleanup GL resources while the context still exists
    engine::ShaderLoader::Instance().Clear();
    engine::MeshLoader::Instance().Clear();
    engine::TextureLoader::Instance().Clear();

    LOG("Exiting...");
    return 0;
}