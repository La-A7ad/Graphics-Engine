#include "Engine/Assets/Loaders/Shader/ShaderLoader.hpp"

namespace engine {

ShaderLoader& ShaderLoader::Instance() {
    static ShaderLoader instance;
    return instance;
}

ShaderLoader::~ShaderLoader() {
    Clear();
}

Shader* ShaderLoader::Load(const std::string& name, const std::string& vertPath, const std::string& fragPath) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        return it->second;
    }
    
    Shader* shader = new Shader(vertPath.c_str(), fragPath.c_str());
    shaders[name] = shader;
    return shader;
}

Shader* ShaderLoader::Get(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        return it->second;
    }
    return nullptr;
}

void ShaderLoader::Clear() {
    for (auto& pair : shaders) {
        delete pair.second;
    }
    shaders.clear();
}

}