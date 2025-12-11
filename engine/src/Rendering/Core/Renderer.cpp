#include "Engine/Rendering/Core/Renderer.hpp"
#include "Engine/ECS/Components/Camera/CameraComponent.hpp"
#include "Engine/ECS/Components/Rendering/MeshRendererComponent.hpp"
#include "Engine/ECS/Core/World/World.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

namespace engine {

Renderer::Renderer() 
    : shader(nullptr), width(800), height(600), initialized(false) {
}

Renderer::~Renderer() {
    if (shader) {
        delete shader;
        shader = nullptr;
    }
}

bool Renderer::Init() {
    shader = new Shader("game/assets/shaders/basic.vert", 
                       "game/assets/shaders/basic.frag");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    initialized = true;
    return true;
}

void Renderer::RenderFrame(float time) {
    if (!initialized) return;
    
    glClearColor(0.18f, 0.23f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(World* world, CameraComponent* camera) {
    if (!initialized || !world || !camera) {
        std::cerr << "Renderer::Render - Invalid parameters (world or camera is null)\n";
        return;
    }
    
    glClearColor(0.18f, 0.23f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update camera aspect ratio based on viewport
    camera->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    // Get view and projection matrices from camera
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj = camera->GetProjectionMatrix();
    
    // Collect all mesh renderers in the world
    auto renderers = world->GetComponentsOfType<MeshRendererComponent>();
    
    // Separate into opaque and transparent objects for correct rendering order
    std::vector<MeshRendererComponent*> opaque;
    std::vector<MeshRendererComponent*> transparent;
    
    for (auto* renderer : renderers) {
        // Skip invalid renderers (missing mesh or material)
        if (!renderer || !renderer->mesh || !renderer->material) {
            continue;
        }
        
        // Sort based on transparency flag in material
        if (renderer->material->transparent) {
            transparent.push_back(renderer);
        } else {
            opaque.push_back(renderer);
        }
    }
    
    // Get camera position for distance-based sorting
    glm::vec3 camPos = camera->GetPosition();
    
    // Sort opaque objects front-to-back (optimization: early depth test culling)
    std::sort(opaque.begin(), opaque.end(), [&camPos](auto* a, auto* b) {
        float distA = glm::length(a->entity->GetWorldPosition() - camPos);
        float distB = glm::length(b->entity->GetWorldPosition() - camPos);
        return distA < distB;
    });
    
    // Sort transparent objects back-to-front (required for correct blending)
    std::sort(transparent.begin(), transparent.end(), [&camPos](auto* a, auto* b) {
        float distA = glm::length(a->entity->GetWorldPosition() - camPos);
        float distB = glm::length(b->entity->GetWorldPosition() - camPos);
        return distA > distB;
    });
    
    // Render opaque objects first
    for (auto* renderer : opaque) {
        RenderEntity(renderer, view, proj);
    }
    
    // TODO: Render sky here (Phase 2 requirement)
    
    // Render transparent objects last
    for (auto* renderer : transparent) {
        RenderEntity(renderer, view, proj);
    }
}

void Renderer::RenderEntity(MeshRendererComponent* renderer, 
                           const glm::mat4& view, 
                           const glm::mat4& proj) {
    // CRITICAL: Defensive null checks
    // Even though Render() filters these out, RenderEntity should be safe on its own
    if (!renderer) {
        std::cerr << "RenderEntity: renderer is null\n";
        return;
    }
    
    if (!renderer->material) {
        std::cerr << "RenderEntity: material is null for entity '" 
                  << (renderer->entity ? renderer->entity->name : "unknown") << "'\n";
        return;
    }
    
    if (!renderer->mesh) {
        std::cerr << "RenderEntity: mesh is null for entity '" 
                  << (renderer->entity ? renderer->entity->name : "unknown") << "'\n";
        return;
    }
    
    if (!renderer->entity) {
        std::cerr << "RenderEntity: entity is null\n";
        return;
    }
    
    // Safe to dereference now
    Material* mat = renderer->material.get(); //cuz raw pointers
    Mesh* mesh = renderer->mesh.get();
    
    // Apply material pipeline state (blending, depth testing, culling, etc.)
    mat->Bind();
    
    // Setup material-specific uniforms (textures, colors, etc.)
    mat->Setup();
    
    // Calculate model matrix from entity's world transform
    glm::mat4 model = renderer->entity->GetWorldTransform();
    
    // Set transformation matrices as uniforms
    // Using value_ptr to get pointer to matrix data for OpenGL
    mat->shader->setMat4("uModel", glm::value_ptr(model));
    mat->shader->setMat4("uView", glm::value_ptr(view));
    mat->shader->setMat4("uProj", glm::value_ptr(proj));
    
    // Draw the mesh with the material's shader
    mesh->Draw(*mat->shader);
}

void Renderer::Resize(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

} // namespace engine