#include "Engine/Core/Graphics/Texture/Texture.hpp"
// REMOVED: #define STB_IMAGE_IMPLEMENTATION
// This is now in STBImageImpl.cpp to avoid multiple definition errors
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
    
    // Flip textures vertically to match OpenGL's texture coordinate system
    // OpenGL expects (0,0) at bottom-left, but image formats use top-left
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        std::cerr << "STB Error: " << stbi_failure_reason() << std::endl;
        return;
    }
    
    // Determine format based on number of channels
    GLenum format = GL_RGB;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    }
    
    // Upload texture data to GPU
    glBindTexture(target, ID);
    glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Generate mipmaps if requested (improves quality at distance)
    if (generateMipmap) {
        glGenerateMipmap(target);
    }
    
    // Set default texture parameters
    // These can be overridden using Sampler objects
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, 
                   generateMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Free image data from CPU memory
    stbi_image_free(data);
    
    std::cout << "Loaded texture: " << path 
              << " (" << width << "x" << height << ", " 
              << channels << " channels)" << std::endl;
}

} // namespace engine