#include "Engine/Rendering/Materials/Implementations/TintedMaterial.hpp"

namespace engine {

TintedMaterial::TintedMaterial() : tint(1.0f) {
}

void TintedMaterial::Setup() {
    if (shader) {
        // FIXED: Pass glm::vec4 directly instead of pointer
        shader->setVec4("uTint", tint);
    }
}

} // namespace engine