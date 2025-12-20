#pragma once

#include "Engine/Rendering/Geometry/Model/Model.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace engine {

/**
 * MeshLoader (actually a Model asset loader):
 * - Owns loaded Model assets (RAII)
 * - Chooses importer by file extension (.obj / .gltf / .glb)
 * - Avoids duplicate loads by caching by *path*
 * - Allows multiple names to reference the same loaded asset
 */
class MeshLoader {
private:
    // Own by path (single owner)
    std::unordered_map<std::string, std::unique_ptr<Model>> modelsByPath;
    // Alias by user-given name (non-owning)
    std::unordered_map<std::string, Model*> modelsByName;

    MeshLoader() = default;

public:
    static MeshLoader& Instance();

    ~MeshLoader();

    Model* Load(const std::string& name, const std::string& path);
    Model* Get(const std::string& name);
    void Clear();
};

} // namespace engine
