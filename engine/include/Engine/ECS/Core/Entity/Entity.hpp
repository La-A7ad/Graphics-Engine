// engine/include/Engine/ECS/Core/Entity/Entity.hpp
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace engine {

class Component;

class Entity {
public:
    std::string name;
    
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    
    Entity* parent;
    std::vector<Entity*> children;
    std::vector<std::unique_ptr<Component>> components;  // CHANGED to unique_ptr
    
    Entity(const std::string& name = "Entity");
    ~Entity();
    
    template<typename T>
    T* AddComponent();
    
    template<typename T>
    T* GetComponent();
    
    void AddChild(Entity* child);
    void RemoveChild(Entity* child);
    
    glm::mat4 GetLocalTransform() const;
    glm::mat4 GetWorldTransform() const;
    glm::vec3 GetWorldPosition() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
};

template<typename T>
T* Entity::AddComponent() {
    auto comp = std::make_unique<T>();
    T* ptr = comp.get();
    ptr->entity = this;
    components.push_back(std::move(comp));
    return ptr;
}

template<typename T>
T* Entity::GetComponent() {
    for (auto& comp : components) {  // CHANGED: auto& instead of auto*
        if (T* casted = dynamic_cast<T*>(comp.get())) {  // CHANGED: comp.get()
            return casted;
        }
    }
    return nullptr;
}

}