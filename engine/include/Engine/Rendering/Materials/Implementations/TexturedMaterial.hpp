#pragma once

#include "Engine/Material.hpp"
#include "Engine/Texture.hpp"
#include "Engine/Sampler.hpp"
#include <glm/glm.hpp>

namespace engine {

class TexturedMaterial : public Material {
public:
    Texture* albedoMap;
    Texture* specularMap;
    Texture* normalMap;
    Texture* emissiveMap;
    
    Sampler* sampler;
    
    glm::vec4 tint;
    
    TexturedMaterial();
    
    void Setup() override;
};

}