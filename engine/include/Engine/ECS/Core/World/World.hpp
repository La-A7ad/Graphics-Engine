// engine/include/Engine/ECS/Core/World/World.hpp
#pragma once

#include <vector>
#include <string>
#include "Engine/ECS/Core/Entity/Entity.hpp"

namespace engine {

class World {
public:
    std::vector<Entity*> entities;
    
    World();
    ~World();
    
    Entity* CreateEntity(const std::string& name = "Entity");
    Entity* FindByName(const std::string& name);
    
    template<typename T>
    std::vector<T*> GetComponentsOfType();
    
    void Clear();
};

template<typename T>
std::vector<T*> World::GetComponentsOfType() {
    std::vector<T*> result;
    for (auto* entity : entities) {
        for (auto& comp : entity->components) {  // CHANGED: auto& instead of auto*
            if (T* casted = dynamic_cast<T*>(comp.get())) {  // CHANGED: comp.get()
                result.push_back(casted);
            }
        }
    }
    return result;
}

}