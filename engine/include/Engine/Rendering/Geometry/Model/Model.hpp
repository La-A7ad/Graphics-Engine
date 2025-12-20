#pragma once

#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include <string>
#include <vector>

namespace engine {

/**
 * Model is a renderable *data structure* (meshes + metadata).
 * Loading/parsing file formats is handled by asset importers (OBJ/GLTF),
 * not by this class.
 */
class Model {
public:
    Model() = default;
    ~Model() = default;

    // Convenience draw (not required by the engine; renderer can also draw meshes directly)
    void Draw(const Shader& shader);

    // Public for now (matches your current usage pattern)
    std::vector<Mesh> meshes;

    // Useful for resolving relative asset paths (e.g., textures referenced by the model)
    std::string directory;

    // Optional: keep the original source path for debugging/caching
    std::string sourcePath;
};

} // namespace engine
