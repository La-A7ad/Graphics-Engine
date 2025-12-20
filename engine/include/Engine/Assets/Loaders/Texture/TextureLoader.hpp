#pragma once

#include "Engine/Texture.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace engine {

class TextureLoader {
private:
    // TextureLoader owns loaded textures (RAII)
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    
    TextureLoader() = default;
    
public:
    static TextureLoader& Instance();

    TextureLoader(const TextureLoader&) = delete;
    TextureLoader& operator=(const TextureLoader&) = delete;
    TextureLoader(TextureLoader&&) = delete;
    TextureLoader& operator=(TextureLoader&&) = delete;

    ~TextureLoader() = default;
    
    Texture* Load(const std::string& name, const std::string& path);
    Texture* Get(const std::string& name);
    void Clear();
};

}
