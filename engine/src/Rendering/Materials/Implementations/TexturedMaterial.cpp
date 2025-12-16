#include "Engine/Rendering/Materials/Implementations/TexturedMaterial.hpp"
#include <iostream>

namespace engine {

TexturedMaterial::TexturedMaterial()
    : albedoMap(nullptr),
      specularMap(nullptr),
      normalMap(nullptr),
      emissiveMap(nullptr),
      sampler(nullptr),
      tint(1.0f) {
    // Default tint is white (1,1,1,1) - no color modification
}

void TexturedMaterial::Setup() {
    // Early return with warning if shader is not set
    if (!shader) {
        std::cerr << "Warning: TexturedMaterial::Setup called with null shader\n";
        return;
    }
    
    // Texture unit counter - OpenGL supports at least 16 texture units (0-15)
    // in OpenGL 3.3 Core Profile
    int unit = 0;
    
    // Bind albedo (diffuse color) texture if available
    if (albedoMap) {
        albedoMap->Bind(unit);
        if (sampler) {
            sampler->Bind(unit);
        }
        shader->setInt("uAlbedoMap", unit);
        unit++;
    }
    
    // Bind specular (shininess) texture if available
    if (specularMap) {
        specularMap->Bind(unit);
        if (sampler) {
            sampler->Bind(unit);
        }
        shader->setInt("uSpecularMap", unit);
        unit++;
    }
    
    // Bind normal map for bump mapping if available
    if (normalMap) {
        normalMap->Bind(unit);
        if (sampler) {
            sampler->Bind(unit);
        }
        shader->setInt("uNormalMap", unit);
        unit++;
    }
    
    // Bind emissive (self-illumination) texture if available
    if (emissiveMap) {
        emissiveMap->Bind(unit);
        if (sampler) {
            sampler->Bind(unit);
        }
        shader->setInt("uEmissiveMap", unit);
        unit++;
    }
    
    // Set tint color uniform (multiplied with texture colors in shader)
    // Note: We set each component individually for compatibility
    // Could also use setVec4 if you implement it in Shader class
    shader->setInt("uAlbedoMap", unit);
    shader->setFloat("uTint.r", tint.r);
    shader->setFloat("uTint.g", tint.g);
    shader->setFloat("uTint.b", tint.b);
    shader->setFloat("uTint.a", tint.a);
    shader->setVec4("uTint", &tint[0]);

}

} // namespace engine