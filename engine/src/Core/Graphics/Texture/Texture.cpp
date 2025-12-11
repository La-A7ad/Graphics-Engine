#include "Engine/Core/Graphics/Texture/Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>

namespace engine {

Texture::Texture(GLenum target) : target(target) {
    glGenTextures(1, &ID);
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

void Texture::Bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, ID);
}

void Texture::Unbind() {
    glBindTexture(target, 0);
}

void Texture::LoadFromFile(const std::string& path, bool generateMipmap) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n";
        return;
    }
    
    GLenum format = GL_RGB;
    if (channels == 1) format = GL_RED;
    else if (channels == 3) format = GL_RGB;
    else if (channels == 4) format = GL_RGBA;
    
    glBindTexture(target, ID);
    glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    if (generateMipmap) {
        glGenerateMipmap(target);
    }
    
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, generateMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(data);
}

}