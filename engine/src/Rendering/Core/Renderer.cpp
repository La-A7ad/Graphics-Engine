// engine/src/Rendering/Core/Renderer.cpp
#include "Engine/Rendering/Core/Renderer.hpp"
#include "Engine/ECS/Components/Camera/CameraComponent.hpp"
#include "Engine/ECS/Components/Rendering/MeshRendererComponent.hpp"
#include "Engine/ECS/Core/World/World.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

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
    if (!initialized || !world || !camera) return;
    
    glClearColor(0.18f, 0.23f, 0.28f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 proj = camera->GetProjectionMatrix();
    
    auto renderers = world->GetComponentsOfType<MeshRendererComponent>();
    
    std::vector<MeshRendererComponent*> opaque;
    std::vector<MeshRendererComponent*> transparent;
    
    for (auto* renderer : renderers) {
        if (!renderer->mesh || !renderer->material) continue;
        
        if (renderer->material->transparent) {  // CHANGED: material-> stays the same (unique_ptr overloads ->)
            transparent.push_back(renderer);
        } else {
            opaque.push_back(renderer);
        }
    }
    
    glm::vec3 camPos = camera->GetPosition();
    
    std::sort(opaque.begin(), opaque.end(), [&camPos](auto* a, auto* b) {
        float distA = glm::length(a->entity->GetWorldPosition() - camPos);
        float distB = glm::length(b->entity->GetWorldPosition() - camPos);
        return distA < distB;
    });
    
    std::sort(transparent.begin(), transparent.end(), [&camPos](auto* a, auto* b) {
        float distA = glm::length(a->entity->GetWorldPosition() - camPos);
        float distB = glm::length(b->entity->GetWorldPosition() - camPos);
        return distA > distB;
    });
    
    for (auto* renderer : opaque) {
        RenderEntity(renderer, view, proj);
    }
    
    for (auto* renderer : transparent) {
        RenderEntity(renderer, view, proj);
    }
}

void Renderer::RenderEntity(MeshRendererComponent* renderer, const glm::mat4& view, const glm::mat4& proj) {
    Material* mat = renderer->material.get();  // CHANGED: .get() to get raw pointer
    Mesh* mesh = renderer->mesh;  // This is already a raw pointer
    
    mat->Bind();
    mat->Setup();
    
    glm::mat4 model = renderer->entity->GetWorldTransform();
    
    mat->shader->setMat4("uModel", glm::value_ptr(model));
    mat->shader->setMat4("uView", glm::value_ptr(view));
    mat->shader->setMat4("uProj", glm::value_ptr(proj));
    
    mesh->Draw(*mat->shader);
}

void Renderer::Resize(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

}