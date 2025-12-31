#pragma once

#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include <string>
#include <vector>

namespace engine {

/**
 * Embedded material data extracted from model files (glTF, etc.)
 * This holds the raw data; actual Material objects are created by the SceneLoader.
 */
struct EmbeddedMaterialData {
    std::string name;
    std::string baseColorTexturePath;  // Relative to model directory
    std::string metallicRoughnessTexturePath;
    std::string normalTexturePath;
    std::string emissiveTexturePath;
    
    float metallicFactor = 1.0f;
    float roughnessFactor = 1.0f;
    float baseColorR = 1.0f, baseColorG = 1.0f, baseColorB = 1.0f, baseColorA = 1.0f;
    
    bool doubleSided = false;
    bool hasAlphaCutoff = false;
    float alphaCutoff = 0.5f;
    std::string alphaMode = "OPAQUE";  // OPAQUE, MASK, or BLEND
};

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
    
    // Embedded material data per mesh (index corresponds to meshes vector)
    std::vector<EmbeddedMaterialData> embeddedMaterials;

    // Useful for resolving relative asset paths (e.g., textures referenced by the model)
    std::string directory;

    // Optional: keep the original source path for debugging/caching
    std::string sourcePath;
};

} // namespace engine