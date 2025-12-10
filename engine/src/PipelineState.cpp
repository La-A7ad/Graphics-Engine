#include "Engine/PipelineState.hpp"

namespace engine {

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

void PipelineState::Apply() const {
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
}

}