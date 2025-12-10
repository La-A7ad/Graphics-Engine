#pragma once

#include <glad/glad.h>
#include <string>

namespace engine {

class Texture {
private:
    GLuint ID;
    GLenum target;
    
public:
    Texture(GLenum target = GL_TEXTURE_2D);
    ~Texture();
    
    void Bind(int unit);
    void Unbind();
    
    void LoadFromFile(const std::string& path, bool generateMipmap = true);
    
    GLuint GetID() const { return ID; }
};

}