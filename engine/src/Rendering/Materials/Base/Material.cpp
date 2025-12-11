#include "Engine/Rendering/Materials/Base/Material.hpp"

namespace engine {

Material::Material() : shader(nullptr), transparent(false) {
}

void Material::Bind() {
    pipelineState.Apply();  // Now skips redundant calls
    if (shader) {
        shader->use();
    }
}

}