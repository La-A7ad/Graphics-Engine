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
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<Entity>> children;
    
    Entity(const std::string& name = "Entity");
    ~Entity();
    
    template<typename T>
    T* AddComponent();
    
    template<typename T>
    T* GetComponent();
    
    void AddChild(Entity* child);
    bool RemoveChild(Entity* child);
    
    glm::mat4 GetLocalTransform() const;
    glm::mat4 GetWorldTransform() const;
    glm::vec3 GetWorldPosition() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
};

template<typename T>
T* Entity::AddComponent() {
    T* comp = new T();
    comp->entity = this;
    components.push_back(comp);
    return comp;
}

template<typename T>
T* Entity::GetComponent() {
    for (auto* comp : components) {
        if (T* casted = dynamic_cast<T*>(comp)) {
            return casted;
        }
    }
    return nullptr;
}

}