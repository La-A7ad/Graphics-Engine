#pragma once

#include "Engine/Model.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace engine {

class MeshLoader {
private:
    // MeshLoader owns loaded models (RAII)
    std::unordered_map<std::string, std::unique_ptr<Model>> models;
    
    MeshLoader() = default;
    
public:
    static MeshLoader& Instance();

    MeshLoader(const MeshLoader&) = delete;
    MeshLoader& operator=(const MeshLoader&) = delete;
    MeshLoader(MeshLoader&&) = delete;
    MeshLoader& operator=(MeshLoader&&) = delete;

    ~MeshLoader() = default;
    
    Model* Load(const std::string& name, const std::string& path);
    Model* Get(const std::string& name);
    void Clear();
};

}
