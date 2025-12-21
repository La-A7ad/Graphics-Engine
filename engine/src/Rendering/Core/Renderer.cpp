#include "Engine/Rendering/Core/Renderer.hpp"
#include "Engine/ECS/Core/World/World.hpp"                // so we can access world->entities
#include "Engine/ECS/Core/Entity/Entity.hpp"
#include "Engine/ECS/Components/Rendering/MeshRendererComponent.hpp"
#include "Engine/ECS/Components/Camera/CameraComponent.hpp"      // so we can call GetProjectionMatrix / GetViewMatrix
#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>            // glm::value_ptr for setMat4


namespace engine {

        Renderer::Renderer()
    : width(320), height(240), initialized(false), m_Window(nullptr) {
    // vao/vbo are nullptr initially
}

Renderer::~Renderer() {
    // Destroy GL objects *before* killing the context
    vao.reset();
    vbo.reset();

    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
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

    // NOW OpenGL + GLAD are ready -> safe to create RAII GL objects
    vao = std::make_unique<VAO>();
    vbo = std::make_unique<VBO>();

    initialized = true;
    return true;
}


   void Renderer::Render(World* world, CameraComponent* camera) {
    if (!initialized || !world || !camera) {
        return;
    }

    // Get actual framebuffer size every frame
    int fbWidth = 0, fbHeight = 0;
    glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
    if (fbWidth <= 0 || fbHeight <= 0) return;

    width  = fbWidth;
    height = fbHeight;

    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Keep camera aspect in sync with framebuffer size
    camera->aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    // Camera matrices
    glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 view = camera->GetViewMatrix();

    

    // Loop over entities
    for (engine::Entity* entity : world->entities) {
        if (!entity) continue;

        auto* meshRenderer = entity->GetComponent<engine::MeshRendererComponent>();
        if (!meshRenderer) continue;
        if (!meshRenderer->mesh) continue;
        if (!meshRenderer->material) continue;

        engine::Material* material = meshRenderer->material.get();
        if (!material->shader) continue;

        engine::Shader* shader = material->shader.get();

        glm::mat4 model = entity->GetWorldTransform();

        // Bind state & shader
        material->Bind();

        // IMPORTANT: these names must match the GLSL uniforms
        shader->setMat4("uProj",  glm::value_ptr(projection));
        shader->setMat4("uView",  glm::value_ptr(view));
        shader->setMat4("uModel", glm::value_ptr(model));

        // ---- PSX shader knobs (harmless if uniforms don't exist) ----
        shader->setVec2("uViewportSize", glm::vec2((float)width, (float)height));

        // PS1-ish snap grid (tweak)
        shader->setVec2("uSnapRes", glm::vec2((float)width * 0.5f, (float)height * 0.5f));
        shader->setFloat("uSnapStrength", 1.0f);

        // Optional: posterize + dithering (only affects PSX frag shader)
        shader->setFloat("uColorSteps", 31.0f);
        shader->setFloat("uDitherStrength", 0.35f);


        // Material-specific uniforms (tint, etc.)
        material->Setup();

        // Draw mesh
        meshRenderer->mesh->Draw(*shader);
    }
}



}