#pragma once

#include "Engine/Model.hpp"
#include <unordered_map>
#include <string>

namespace engine {

class MeshLoader {
private:
    std::unordered_map<std::string, Model*> models;
    
    MeshLoader() = default;
    
public:
    static MeshLoader& Instance();
    
    ~MeshLoader();
    
    Model* Load(const std::string& name, const std::string& path);
    Model* Get(const std::string& name);
    void Clear();
};

}