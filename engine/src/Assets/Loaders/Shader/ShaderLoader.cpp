// Engine/Assets/Loaders/Shader/ShaderLoader.cpp
#include "Engine/Assets/Loaders/Shader/ShaderLoader.hpp"
#include <iostream>

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
    
    try {
        Shader* shader = new Shader(vertPath.c_str(), fragPath.c_str());
        shaders[name] = shader;
        std::cout << "✓ Loaded shader: " << name << "\n";
        return shader;
    } catch (const ShaderCompilationError& e) {
        std::cerr << "✗ Failed to load shader '" << name << "': " << e.what() << "\n";
        return nullptr;
    }
}

Shader* ShaderLoader::Get(const std::string& name) {
    auto it = shaders.find(name);
    return (it != shaders.end()) ? it->second : nullptr;
}

void ShaderLoader::Clear() {
    for (auto& pair : shaders) {
        delete pair.second;
    }
    shaders.clear();
}

}