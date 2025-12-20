#include "Engine/Assets/Loaders/Mesh/MeshLoader.hpp"

#include <memory>

namespace engine {

MeshLoader& MeshLoader::Instance() {
    static MeshLoader instance;
    return instance;
}

Model* MeshLoader::Load(const std::string& name, const std::string& path) {
    auto it = models.find(name);
    if (it != models.end()) {
        return it->second.get();
    }

    auto model = std::make_unique<Model>(path);
    Model* raw = model.get();
    models.emplace(name, std::move(model));
    return raw;
}

Model* MeshLoader::Get(const std::string& name) {
    auto it = models.find(name);
    if (it != models.end()) {
        return it->second.get();
    }
    return nullptr;
}

void MeshLoader::Clear() {
    models.clear();
}

}
