#pragma once

#include "Engine/Component.hpp"
#include "Engine/Mesh.hpp"
#include "Engine/Material.hpp"
#include <memory>

namespace engine {

class MeshRendererComponent : public Component {
public:
    Mesh* mesh; 
    std::unique_ptr<Material> material;  
    
    MeshRendererComponent();
    ~MeshRendererComponent() = default;  //unique_ptr handles cleanup automatically
};

}