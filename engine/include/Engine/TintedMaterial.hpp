#pragma once

#include "Engine/Material.hpp"
#include <glm/glm.hpp>

namespace engine {

class TintedMaterial : public Material {
public:
    glm::vec4 tint;
    
    TintedMaterial();
    
    void Setup() override;
};

}