// engine/include/Engine/ECS/Components/Rendering/MeshRendererComponent.hpp
#pragma once

#include "Engine/Component.hpp"
#include "Engine/Mesh.hpp"
#include "Engine/Material.hpp"
#include <memory>

namespace engine {

class MeshRendererComponent : public Component {
public:
    Mesh* mesh;  // Non-owning - points to mesh in Model owned by MeshLoader
    std::unique_ptr<Material> material;  // Owning - this component owns the material
    
    MeshRendererComponent();
};

}