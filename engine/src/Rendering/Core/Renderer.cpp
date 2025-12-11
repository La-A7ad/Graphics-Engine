#include "Engine/Renderer.hpp"
#include "Engine/World.hpp"                // so we can access world->entities
#include "Engine/Entity.hpp"
#include "Engine/MeshRendererComponent.hpp"
#include "Engine/CameraComponent.hpp"      // so we can call GetProjectionMatrix / GetViewMatrix
#include "Engine/Shader.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>            // glm::value_ptr for setMat4


namespace engine {

    Renderer::Renderer() 
        : width(800), height(600), initialized(false), m_Window(nullptr) {
    }

    Renderer::~Renderer() {
        if (m_Window) {
            glfwDestroyWindow(m_Window);
        }
        glfwTerminate();
    }

    bool Renderer::Init() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, "Graphics Engine", nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        glEnable(GL_DEPTH_TEST);
        // glCullFace(GL_BACK); // Optional: Enable if your geometry is winding correctly

        initialized = true;
        return true;
    }

   void Renderer::Render(World* world, CameraComponent* camera) {
    if (!initialized || !world || !camera) {
        return;
    }

    // Clear frame
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. Camera matrices
    glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 view       = camera->GetViewMatrix();   // uses Entity's transform internally

    // 2. Iterate over all entities in the world
    for (engine::Entity* entity : world->entities) {
        if (!entity) continue;

        // Only render entities with a MeshRendererComponent
        auto* meshRenderer = entity->GetComponent<engine::MeshRendererComponent>();
        if (!meshRenderer) continue;

        // Make sure mesh + material + shader exist
        if (!meshRenderer->mesh) continue;
        if (!meshRenderer->material) continue;
        engine::Material* material = meshRenderer->material.get();
        if (!material || !material->shader) continue;

        engine::Shader* shader = material->shader.get();

        // 3. Model matrix from Entity's built-in transform
        glm::mat4 model = entity->GetWorldTransform();

        // 4. Bind pipeline state + shader
        material->Bind();  // calls pipelineState.Apply() + shader->use()

        // 5. Set standard matrices – match your GLSL uniforms
        shader->setMat4("uProj",  glm::value_ptr(projection));
        shader->setMat4("uView",  glm::value_ptr(view));
        shader->setMat4("uModel", glm::value_ptr(model));

        // 6. Material-specific uniforms (tint, textures, etc.)
        material->Setup();

        // 7. Draw the mesh
        meshRenderer->mesh->Draw(*shader);
    }
}


}