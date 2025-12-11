#include "Engine/Core/Graphics/State/PipelineState.hpp"

namespace engine {

// Static member definition
const PipelineState* PipelineState::currentState = nullptr;

PipelineState::PipelineState()
    : faceCulling(true),
      cullFace(GL_BACK),
      depthTesting(true),
      depthFunc(GL_LESS),
      depthMask(true),
      blending(false),
      blendSrc(GL_SRC_ALPHA),
      blendDst(GL_ONE_MINUS_SRC_ALPHA),
      colorMask{true, true, true, true} {
}

bool PipelineState::operator==(const PipelineState& other) const {
    return faceCulling == other.faceCulling &&
           cullFace == other.cullFace &&
           depthTesting == other.depthTesting &&
           depthFunc == other.depthFunc &&
           depthMask == other.depthMask &&
           blending == other.blending &&
           blendSrc == other.blendSrc &&
           blendDst == other.blendDst &&
           colorMask[0] == other.colorMask[0] &&
           colorMask[1] == other.colorMask[1] &&
           colorMask[2] == other.colorMask[2] &&
           colorMask[3] == other.colorMask[3];
}

void PipelineState::Apply() const {
    // Only apply if state has changed
    if (currentState == this) {
        return;  // Already applied, skip redundant calls
    }
    
    // Apply state changes
    if (faceCulling) {
        glEnable(GL_CULL_FACE);
        glCullFace(cullFace);
    } else {
        glDisable(GL_CULL_FACE);
    }
    
    if (depthTesting) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(depthFunc);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glDepthMask(depthMask ? GL_TRUE : GL_FALSE);
    
    if (blending) {
        glEnable(GL_BLEND);
        glBlendFunc(blendSrc, blendDst);
    } else {
        glDisable(GL_BLEND);
    }
    
    glColorMask(colorMask[0], colorMask[1], colorMask[2], colorMask[3]);
    
    // Mark this state as current
    currentState = this;
}

}