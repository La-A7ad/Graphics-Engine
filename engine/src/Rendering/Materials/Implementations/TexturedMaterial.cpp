#include "Engine/Rendering/Materials/Implementations/TexturedMaterial.hpp"

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
    
    // Bind textures to their assigned units
    if (albedoMap) {
        albedoMap->Bind(albedoUnit);
        if (sampler) sampler->Bind(albedoUnit);
        shader->setInt("uAlbedoMap", albedoUnit);
    }
    
    if (specularMap) {
        specularMap->Bind(specularUnit);
        if (sampler) sampler->Bind(specularUnit);
        shader->setInt("uSpecularMap", specularUnit);
    }
    
    if (normalMap) {
        normalMap->Bind(normalUnit);
        if (sampler) sampler->Bind(normalUnit);
        shader->setInt("uNormalMap", normalUnit);
    }
    
    if (emissiveMap) {
        emissiveMap->Bind(emissiveUnit);
        if (sampler) sampler->Bind(emissiveUnit);
        shader->setInt("uEmissiveMap", emissiveUnit);
    }
    
    // Set tint uniform
    shader->setFloat("uTint.r", tint.r);
    shader->setFloat("uTint.g", tint.g);
    shader->setFloat("uTint.b", tint.b);
    shader->setFloat("uTint.a", tint.a);
}

}