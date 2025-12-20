#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Engine/Entity.hpp"

namespace engine {

class World {
public:
    //changed this to unique pointer
    std::vector<std::unique_ptr<Entity>> entities;


    //Rule of Five

    World() = default;
    ~World() = default;
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) noexcept = default;
    World& operator=(World&&) noexcept = default;

    //
    
    Entity* CreateEntity(const std::string& name = "Entity");
    Entity* FindByName(const std::string& name);
    
    template<typename T>
    std::vector<T*> GetComponentsOfType();
    
    void Clear();
};

template<typename T>
std::vector<T*> World::GetComponentsOfType() {
    std::vector<T*> result;
    for (auto& entityPtr : entities) {
        Entity* entity = entityPtr.get();
        if (!entity) continue;
        for (auto& comp : entity->components) {
            if (T* casted = dynamic_cast<T*>(comp.get())) {
                result.push_back(casted);
            }
        }
    }
    return result;
}

}
