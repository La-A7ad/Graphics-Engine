#include "Engine/TextureLoader.hpp"

namespace engine {

TextureLoader& TextureLoader::Instance() {
    static TextureLoader instance;
    return instance;
}

TextureLoader::~TextureLoader() {
    Clear();
}

Texture* TextureLoader::Load(const std::string& name, const std::string& path) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    
    Texture* texture = new Texture();
    texture->LoadFromFile(path);
    textures[name] = texture;
    return texture;
}

Texture* TextureLoader::Get(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

void TextureLoader::Clear() {
    for (auto& pair : textures) {
        delete pair.second;
    }
    textures.clear();
}

}