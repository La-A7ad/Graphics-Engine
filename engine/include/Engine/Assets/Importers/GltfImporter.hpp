#pragma once

#include "Engine/Rendering/Geometry/Model/Model.hpp"
#include <memory>
#include <string>

namespace engine {

/**
 * glTF importer: parses .gltf/.glb files into an engine::Model data structure.
 *
 * Current scope (minimal, but useful):
 *  - Mesh primitives (triangles)
 *  - POSITION / NORMAL / TEXCOORD_0
 *  - Indices (optional)
 *
 * Future scope (animations):
 *  - Node hierarchy, skins, JOINTS_0/WEIGHTS_0, animation channels/samplers.
 */
class GltfImporter {
public:
    static std::unique_ptr<Model> Import(const std::string& path);
};

} // namespace engine
