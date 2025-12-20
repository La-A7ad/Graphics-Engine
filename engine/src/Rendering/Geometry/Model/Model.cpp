#include "Engine/Rendering/Geometry/Model/Model.hpp"

namespace engine {

void Model::Draw(const Shader& shader) {
    for (auto& mesh : meshes) {
        mesh.Draw(shader);
    }
}

} // namespace engine
