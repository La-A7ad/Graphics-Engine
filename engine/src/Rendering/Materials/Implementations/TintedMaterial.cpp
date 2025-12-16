#include "Engine/Rendering/Materials/Implementations/TintedMaterial.hpp"

namespace engine {

TintedMaterial::TintedMaterial() : tint(1.0f) {
}

void TintedMaterial::Setup() {
    if (shader) {
        shader->setFloat("uTint.r", tint.r);
        shader->setFloat("uTint.g", tint.g);
        shader->setFloat("uTint.b", tint.b);
        shader->setFloat("uTint.a", tint.a);
        shader->setVec4("uTint", &tint[0]);

    }
}

}