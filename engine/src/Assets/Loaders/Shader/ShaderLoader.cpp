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

Shader* ShaderLoader::Get(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        return it->second.get();
    }
    return nullptr;
}

void ShaderLoader::Clear() {
    shaders.clear();   // shared_ptr counts drop to 0 -> ~Shader() deletes GL program
}

}