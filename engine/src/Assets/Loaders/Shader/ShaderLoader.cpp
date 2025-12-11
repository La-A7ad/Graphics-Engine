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

Shader* ShaderLoader::Load(const std::string& name,
                           const std::string& vertPath,
                           const std::string& fragPath) {
    auto shader = std::make_shared<Shader>(vertPath.c_str(), fragPath.c_str());
    shaders[name] = shader;
    std::cout << "✓ Loaded shader: " << name << "\n";
    return shader.get();  // Return raw pointer for compatibility
}

}