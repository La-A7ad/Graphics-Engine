// engine/src/ECS/Core/Entity/Entity.cpp
#include "Engine/ECS/Core/Entity/Entity.hpp"
#include "Engine/ECS/Core/Component/Component.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace engine {

Entity::Entity(const std::string& name)
    : name(name), 
      position(0.0f), 
      rotation(0.0f), 
      scale(1.0f),
      parent(nullptr) {
}

Entity::~Entity() {
    // unique_ptr automatically cleans up components
    components.clear();
}

void Entity::AddChild(Entity* child) {
    if (child->parent) {
        child->parent->RemoveChild(child);
    }
    children.push_back(child);
    child->parent = this;
}

void Entity::RemoveChild(Entity* child) {
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->parent = nullptr;
    }
}

glm::mat4 Entity::GetLocalTransform() const {
    glm::mat4 m(1.0f);
    m = glm::translate(m, position);
    m = glm::rotate(m, rotation.x, glm::vec3(1, 0, 0));
    m = glm::rotate(m, rotation.y, glm::vec3(0, 1, 0));
    m = glm::rotate(m, rotation.z, glm::vec3(0, 0, 1));
    m = glm::scale(m, scale);
    return m;
}

glm::mat4 Entity::GetWorldTransform() const {
    if (parent) {
        return parent->GetWorldTransform() * GetLocalTransform();
    }
    return GetLocalTransform();
}

glm::vec3 Entity::GetWorldPosition() const {
    glm::mat4 world = GetWorldTransform();
    return glm::vec3(world[3]);
}

glm::vec3 Entity::GetForward() const {
    glm::mat4 world = GetWorldTransform();
    return glm::normalize(-glm::vec3(world[2]));
}

glm::vec3 Entity::GetRight() const {
    glm::mat4 world = GetWorldTransform();
    return glm::normalize(glm::vec3(world[0]));
}

glm::vec3 Entity::GetUp() const {
    glm::mat4 world = GetWorldTransform();
    return glm::normalize(glm::vec3(world[1]));
}

}