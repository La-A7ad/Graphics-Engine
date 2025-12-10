#include "Engine/Sampler.hpp"

namespace engine {

Sampler::Sampler() {
    glGenSamplers(1, &ID);
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