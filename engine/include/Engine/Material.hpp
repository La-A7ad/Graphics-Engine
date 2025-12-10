#pragma once

#include "Engine/Shader.hpp"
#include "Engine/PipelineState.hpp"

namespace engine {

class Material {
public:
    Shader* shader;
    PipelineState pipelineState;
    bool transparent;
    
    Material();
    virtual ~Material() = default;
    
    virtual void Setup() = 0;
    
    void Bind();
};

}