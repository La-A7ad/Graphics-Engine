#pragma once

#include "Engine/Texture.hpp"
#include <unordered_map>
#include <string>

namespace engine {

class TextureLoader {
private:
    std::unordered_map<std::string, Texture*> textures;
    
    TextureLoader() = default;
    
public:
    static TextureLoader& Instance();
    
    ~TextureLoader();
    
    Texture* Load(const std::string& name, const std::string& path);
    Texture* Get(const std::string& name);
    void Clear();
};

}