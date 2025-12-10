#include "Engine/TexturedMaterial.hpp"

namespace engine {

TexturedMaterial::TexturedMaterial()
    : albedoMap(nullptr),
      specularMap(nullptr),
      normalMap(nullptr),
      emissiveMap(nullptr),
      sampler(nullptr),
      tint(1.0f) {
}

void TexturedMaterial::Setup() {
    if (!shader) return;
    
    int unit = 0;
    
    if (albedoMap) {
        albedoMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uAlbedoMap", unit++);
    }
    
    if (specularMap) {
        specularMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uSpecularMap", unit++);
    }
    
    if (normalMap) {
        normalMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uNormalMap", unit++);
    }
    
    if (emissiveMap) {
        emissiveMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uEmissiveMap", unit++);
    }
    
    shader->setFloat("uTint.r", tint.r);
    shader->setFloat("uTint.g", tint.g);
    shader->setFloat("uTint.b", tint.b);
    shader->setFloat("uTint.a", tint.a);
}

}