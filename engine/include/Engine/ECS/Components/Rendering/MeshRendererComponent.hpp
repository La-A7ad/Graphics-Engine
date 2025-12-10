#pragma once

#include "Engine/Component.hpp"
#include "Engine/Mesh.hpp"
#include "Engine/Material.hpp"

namespace engine {

class MeshRendererComponent : public Component {
public:
    Mesh* mesh;
    Material* material;
    
    MeshRendererComponent();
};

}