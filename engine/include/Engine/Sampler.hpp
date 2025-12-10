#pragma once

#include <glad/glad.h>

namespace engine {

class Sampler {
private:
    GLuint ID;
    
public:
    Sampler();
    ~Sampler();
    
    void Bind(int unit);
    void Unbind(int unit);
    
    void SetMinFilter(GLenum filter);
    void SetMagFilter(GLenum filter);
    void SetWrapS(GLenum wrap);
    void SetWrapT(GLenum wrap);
    void SetWrapR(GLenum wrap);
};

}