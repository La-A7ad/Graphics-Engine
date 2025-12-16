#pragma once

#include "Engine/Shader.hpp"
#include <unordered_map>
#include <memory>
#include <string>

namespace engine {

class ShaderLoader {
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    
    ShaderLoader() = default;
    
public:
    static ShaderLoader& Instance();
    
    ~ShaderLoader();
    
    Shader* Load(const std::string& name, const std::string& vertPath, const std::string& fragPath);
    Shader* Get(const std::string& name);
    void Clear();
};

}