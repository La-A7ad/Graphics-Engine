#pragma once

#include "Engine/ECS/Core/World/World.hpp"
#include "Engine/ECS/Core/Entity/Entity.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <memory>

namespace engine {

/**
 * SceneLoader - Deserializes scene from JSON files
 * 
 * Handles loading:
 * - Assets (shaders, textures, models)
 * - Entities with components
 * - Scene graph (parent-child relationships)
 * - Materials and pipeline states
 */
class SceneLoader {
public:
    /**
     * Load a complete scene from a JSON file
     * @param path Path to scene JSON file
     * @return Loaded world (nullptr on failure)
     */
    static std::unique_ptr<World> LoadScene(const std::string& path);
    
private:
    // Asset loading helpers
    static void LoadAssets(const nlohmann::json& assetsJson, const std::string& sceneDir);
    static void LoadShaders(const nlohmann::json& shadersJson, const std::string& sceneDir);
    static void LoadTextures(const nlohmann::json& texturesJson, const std::string& sceneDir);
    static void LoadModels(const nlohmann::json& modelsJson, const std::string& sceneDir);
    
    // Entity loading helpers
    static void LoadEntities(World* world, const nlohmann::json& entitiesJson);
    static Entity* LoadEntity(World* world, const nlohmann::json& entityJson, Entity* parent);
    
    // Component loading helpers
    static void LoadComponents(Entity* entity, const nlohmann::json& componentsJson);
    static void LoadCameraComponent(Entity* entity, const nlohmann::json& camJson);
    static void LoadMeshRendererComponent(Entity* entity, const nlohmann::json& rendererJson);
    
    // Material loading helpers
    static std::unique_ptr<class Material> LoadMaterial(const nlohmann::json& matJson);
    static void LoadPipelineState(class PipelineState& state, const nlohmann::json& stateJson);
    
    // Utility functions
    static std::string GetSceneDirectory(const std::string& scenePath);
    static std::string ResolvePath(const std::string& path, const std::string& sceneDir);
};

} // namespace engine