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
    if (!shader) {
        std::cerr << "Warning: TexturedMaterial::Setup called with null shader\n";
        return;
    }

    shader->use();

    int unit = 0;

    // Albedo must always match the unit we bind it to
    if (albedoMap) {
        albedoMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uAlbedoMap", unit);
        unit++;
    } else {
        // If you want: avoid sampling garbage
        shader->setInt("uAlbedoMap", 0);
    }

    if (specularMap) {
        specularMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uSpecularMap", unit);
        unit++;
    }

    if (normalMap) {
        normalMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uNormalMap", unit);
        unit++;
    }

    if (emissiveMap) {
        emissiveMap->Bind(unit);
        if (sampler) sampler->Bind(unit);
        shader->setInt("uEmissiveMap", unit);
        unit++;
    }

    // FIXED: Pass glm::vec4 directly instead of pointer
    shader->setVec4("uTint", tint);
}

} // namespace engine