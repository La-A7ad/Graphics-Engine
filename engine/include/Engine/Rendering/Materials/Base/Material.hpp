#pragma once

#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include "Engine/Core/Graphics/State/PipelineState.hpp"
#include <memory>

namespace engine {

class Material {
public:
    std::shared_ptr<Shader> shader;  
    PipelineState pipelineState;
    bool transparent;
    
    Material();
    virtual ~Material() = default;
    
    virtual void Setup() = 0;
    
    void Bind();
};

}