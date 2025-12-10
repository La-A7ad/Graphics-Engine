#include "Engine/MeshLoader.hpp"

namespace engine {

MeshLoader& MeshLoader::Instance() {
    static MeshLoader instance;
    return instance;
}

MeshLoader::~MeshLoader() {
    Clear();
}

Model* MeshLoader::Load(const std::string& name, const std::string& path) {
    auto it = models.find(name);
    if (it != models.end()) {
        return it->second;
    }
    
    Model* model = new Model(path);
    models[name] = model;
    return model;
}

Model* MeshLoader::Get(const std::string& name) {
    auto it = models.find(name);
    if (it != models.end()) {
        return it->second;
    }
    return nullptr;
}

void MeshLoader::Clear() {
    for (auto& pair : models) {
        delete pair.second;
    }
    models.clear();
}

}