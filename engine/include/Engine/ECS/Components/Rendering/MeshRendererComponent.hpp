// engine/include/Engine/ECS/Components/Rendering/MeshRendererComponent.hpp
#pragma once

#include "Engine/ECS/Core/Component/Component.hpp"
#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include "Engine/Rendering/Materials/Base/Material.hpp"
#include <memory>

namespace engine {

class MeshRendererComponent : public Component {
public:
    Mesh* mesh;  // Non-owning - points to mesh in Model owned by MeshLoader
    std::unique_ptr<Material> material;  // Owning - this component owns the material
    
    MeshRendererComponent();
};

}