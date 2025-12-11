#include "Engine/ECS/Core/World/World.hpp"

namespace engine {

World::World() {
}

World::~World() {
    Clear();
}

Entity* World::CreateEntity(const std::string& name) {
    Entity* entity = new Entity(name);
    entities.push_back(entity);
    return entity;
}

Entity* World::FindByName(const std::string& name) {
    for (auto* entity : entities) {
        if (entity->name == name) {
            return entity;
        }
    }
    return nullptr;
}

void World::Clear() {
    for (auto* entity : entities) {
        delete entity;
    }
    entities.clear();
}

}