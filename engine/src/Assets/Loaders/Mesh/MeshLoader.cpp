#include "Engine/Assets/Loaders/Mesh/MeshLoader.hpp"

#include "Engine/Assets/Importers/ObjImporter.hpp"
#include "Engine/Assets/Importers/GltfImporter.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace engine {

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return (char)std::tolower(c); });
    return s;
}

static std::string getExtension(const std::string& path) {
    const auto dot = path.find_last_of('.');
    if (dot == std::string::npos) return "";
    return toLower(path.substr(dot + 1));
}

MeshLoader& MeshLoader::Instance() {
    static MeshLoader instance;
    return instance;
}

MeshLoader::~MeshLoader() {
    Clear();
}

Model* MeshLoader::Load(const std::string& name, const std::string& path) {
    // If this name already exists, return it.
    auto nameIt = modelsByName.find(name);
    if (nameIt != modelsByName.end()) {
        return nameIt->second;
    }

    // If this path is already loaded, alias it with this name.
    auto pathIt = modelsByPath.find(path);
    if (pathIt != modelsByPath.end()) {
        modelsByName[name] = pathIt->second.get();
        return pathIt->second.get();
    }

    // Choose importer by extension
    std::unique_ptr<Model> imported;
    const std::string ext = getExtension(path);

    if (ext == "obj") {
        imported = ObjImporter::Import(path);
    } else if (ext == "gltf" || ext == "glb") {
        imported = GltfImporter::Import(path);
    } else {
        std::cerr << "MeshLoader::Load: unsupported model extension '" << ext
                  << "' for path: " << path << "\n";
        return nullptr;
    }

    if (!imported) {
        std::cerr << "MeshLoader::Load: failed to import model: " << path << "\n";
        return nullptr;
    }

    Model* raw = imported.get();
    modelsByPath.emplace(path, std::move(imported));
    modelsByName[name] = raw;
    return raw;
}

Model* MeshLoader::Get(const std::string& name) {
    auto it = modelsByName.find(name);
    if (it != modelsByName.end()) {
        return it->second;
    }
    return nullptr;
}

void MeshLoader::Clear() {
    modelsByName.clear();
    modelsByPath.clear(); // unique_ptr frees models
}

} // namespace engine
