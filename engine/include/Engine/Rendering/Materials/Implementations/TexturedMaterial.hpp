#pragma once

#include "Engine/Material.hpp"
#include "Engine/Texture.hpp"
#include "Engine/Sampler.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace engine {

class TexturedMaterial : public Material {
public:
    Texture* albedoMap;      // Non-owning (from TextureLoader)
    Texture* specularMap;
    Texture* normalMap;
    Texture* emissiveMap;
    
    std::shared_ptr<Sampler> sampler;  // Can be shared between materials
    
    glm::vec4 tint;
    
    // Explicit texture unit assignments (makes conflicts obvious)
    int albedoUnit = 0;
    int specularUnit = 1;
    int normalUnit = 2;
    int emissiveUnit = 3;
    
    TexturedMaterial();
    
    void Setup() override;
};

}