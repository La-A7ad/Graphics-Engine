#include "Engine/Core/Graphics/Texture/Sampler.hpp"

namespace engine {

Sampler::Sampler() {
    glGenSamplers(1, &ID);

    // default sampling
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_R, GL_REPEAT);

    // degault filtering
    glSamplerParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glSamplerParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


Sampler::~Sampler() {
    glDeleteSamplers(1, &ID);
}

void Sampler::Bind(int unit) {
    glBindSampler(unit, ID);
}

void Sampler::Unbind(int unit) {
    glBindSampler(unit, 0);
}

void Sampler::SetMinFilter(GLenum filter) {
    glSamplerParameteri(ID, GL_TEXTURE_MIN_FILTER, filter);
}

void Sampler::SetMagFilter(GLenum filter) {
    glSamplerParameteri(ID, GL_TEXTURE_MAG_FILTER, filter);
}

void Sampler::SetWrapS(GLenum wrap) {
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_S, wrap);
}

void Sampler::SetWrapT(GLenum wrap) {
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_T, wrap);
}

void Sampler::SetWrapR(GLenum wrap) {
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_R, wrap);
}

}