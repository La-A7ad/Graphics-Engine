#pragma once

#include <glad/glad.h>

namespace engine {

struct PipelineState {
    bool faceCulling;
    GLenum cullFace;
    
    bool depthTesting;
    GLenum depthFunc;
    bool depthMask;
    
    bool blending;
    GLenum blendSrc;
    GLenum blendDst;
    
    bool colorMask[4];
    
    PipelineState();
    void Apply() const;
    
    //compare states
    bool operator==(const PipelineState& other) const;
    bool operator!=(const PipelineState& other) const { return !(*this == other); }
    
    //gloval state trackig
    static const PipelineState* currentState;
    static void SetCurrent(const PipelineState* state) { currentState = state; }
};

}