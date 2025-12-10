#include "Engine/Material.hpp"

namespace engine {

Material::Material() : shader(nullptr), transparent(false) {
}

void Material::Bind() {
    pipelineState.Apply();
    if (shader) {
        shader->use();
    }
}

}