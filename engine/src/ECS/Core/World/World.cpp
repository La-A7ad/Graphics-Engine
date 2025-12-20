#include "Engine/ECS/Core/World/World.hpp"

#include <utility>

namespace engine {

Entity* World::CreateEntity(const std::string& name) {
    auto entity = std::make_unique<Entity>(name);
    Entity* raw = entity.get();
    entities.emplace_back(std::move(entity));
    return raw;
}

Entity* World::FindByName(const std::string& name) {
    for (auto& entityPtr : entities) {
        Entity* entity = entityPtr.get();
        if (entity && entity->name == name) return entity;
    }
    return nullptr;
}

void World::Clear() {
    entities.clear();
}

}
