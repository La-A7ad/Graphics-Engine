#include "Engine/Assets/Loaders/Texture/TextureLoader.hpp"
#include <iostream>

#include <memory>

namespace engine {

TextureLoader& TextureLoader::Instance() {
    static TextureLoader instance;
    return instance;
}

Texture* TextureLoader::Load(const std::string& name, const std::string& path) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second.get();
    }

    auto texture = std::make_unique<Texture>();
    texture->LoadFromFile(path);
    
    // FIX: Add validation to ensure texture was loaded
    // Check if file was found by verifying texture ID is valid
    if (texture->GetID() == 0) {
        std::cerr << "ERROR:  Texture '" << name << "' failed to load from:  " << path << "\n";
        return nullptr;
    }

    Texture* raw = texture.get();
    textures.emplace(name, std::move(texture));
    return raw;
}

Texture* TextureLoader::Get(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second.get();
    }
    return nullptr;
}

void TextureLoader::Clear() {
    textures.clear();
}

}
