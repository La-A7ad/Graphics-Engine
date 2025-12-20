#include "Engine/Rendering/Materials/Implementations/TintedMaterial.hpp"

namespace engine {

TintedMaterial::TintedMaterial() : tint(1.0f) {
}

void TintedMaterial::Setup() {
    if (shader) {
        shader->setVec4("uTint", &tint[0]);

    }
}

}