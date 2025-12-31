#include "Engine/Scene/SceneLoader.hpp"
#include "Engine/Assets/Loaders/Shader/ShaderLoader.hpp"
#include "Engine/Assets/Loaders/Texture/TextureLoader.hpp"
#include "Engine/Assets/Loaders/Mesh/MeshLoader.hpp"
#include "Engine/ECS/Components/Camera/CameraComponent.hpp"
#include "Engine/ECS/Components/Rendering/MeshRendererComponent.hpp"
#include "Engine/Rendering/Materials/Implementations/TintedMaterial.hpp"
#include "Engine/Rendering/Materials/Implementations/TexturedMaterial.hpp"
#include "Engine/Core/Graphics/State/PipelineState.hpp"
#include "Engine/Core/Graphics/Texture/Sampler.hpp"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace engine {

std::string SceneLoader::GetSceneDirectory(const std::string& scenePath) {
    const auto slash = scenePath.find_last_of("/\\");
    if (slash == std::string::npos) return ".";
    return scenePath.substr(0, slash);
}

std::string SceneLoader::ResolvePath(const std::string& path, const std::string& sceneDir) {
    // If absolute path, use as-is
    if (path.empty()) return path;
    if (path[0] == '/' || (path.size() > 1 && path[1] == ':')) {
        return path;
    }
    // Relative path - resolve from scene directory
    return sceneDir + "/" + path;
}

std::unique_ptr<World> SceneLoader::LoadScene(const std::string& path) {
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "Loading scene: " << path << "\n";
    std::cout << "═══════════════════════════════════════\n";
    
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "ERROR: Failed to open scene file: " << path << "\n";
        return nullptr;
    }
    
    json sceneJson;
    try {
        file >> sceneJson;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Failed to parse scene JSON: " << e.what() << "\n";
        return nullptr;
    }
    
    const std::string sceneDir = GetSceneDirectory(path);
    
    // Load assets first
    if (sceneJson.contains("assets")) {
        LoadAssets(sceneJson["assets"], "");
    }
    
    // Create world
    auto world = std::make_unique<World>();
    
    // Load entities
    if (sceneJson.contains("entities")) {
        LoadEntities(world.get(), sceneJson["entities"]);
    }
    
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "✓ Scene loaded successfully\n";
    std::cout << "═══════════════════════════════════════\n\n";
    
    return world;
}

void SceneLoader::LoadAssets(const json& assetsJson, const std::string& sceneDir) {
    std::cout << "\n--- Loading Assets ---\n";
    
    if (assetsJson.contains("shaders")) {
        LoadShaders(assetsJson["shaders"], sceneDir);
    }
    if (assetsJson.contains("textures")) {
        LoadTextures(assetsJson["textures"], sceneDir);
    }
    if (assetsJson.contains("models")) {
        LoadModels(assetsJson["models"], sceneDir);
    }
}

void SceneLoader::LoadShaders(const json& shadersJson, const std::string& sceneDir) {
    auto& loader = ShaderLoader::Instance();
    
    for (const auto& [name, paths] : shadersJson.items()) {
        std::string vertPath = ResolvePath(paths["vertex"].get<std::string>(), sceneDir);
        std::string fragPath = ResolvePath(paths["fragment"].get<std::string>(), sceneDir);
        loader.Load(name, vertPath, fragPath);
    }
}

void SceneLoader::LoadTextures(const json& texturesJson, const std::string& sceneDir) {
    auto& loader = TextureLoader::Instance();
    
    for (const auto& [name, path] : texturesJson.items()) {
        std::string fullPath = ResolvePath(path.get<std::string>(), sceneDir);
        auto* tex = loader.Load(name, fullPath);
        if (tex) {
            std::cout << "✓ Loaded texture: " << name << "\n";
        }
    }
}

void SceneLoader::LoadModels(const json& modelsJson, const std::string& sceneDir) {
    auto& loader = MeshLoader::Instance();
    
    for (const auto& [name, path] : modelsJson.items()) {
        std::string fullPath = ResolvePath(path.get<std::string>(), sceneDir);
        auto* model = loader.Load(name, fullPath);
        if (model) {
            std::cout << "✓ Loaded model: " << name << " (" << model->meshes.size() << " meshes)\n";
        }
    }
}

void SceneLoader::LoadEntities(World* world, const json& entitiesJson) {
    std::cout << "\n--- Loading Entities ---\n";
    
    for (const auto& entityJson : entitiesJson) {
        LoadEntity(world, entityJson, nullptr);
    }
}

Entity* SceneLoader::LoadEntity(World* world, const json& entityJson, Entity* parent) {
    std::string name = entityJson.value("name", "Entity");
    Entity* entity = world->CreateEntity(name);
    
    // Set parent if specified
    if (parent) {
        parent->AddChild(entity);
    }
    
    // Load transform
    if (entityJson.contains("transform")) {
        const auto& t = entityJson["transform"];
        
        if (t.contains("position")) {
            const auto& p = t["position"];
            entity->position = glm::vec3(p[0].get<float>(), p[1].get<float>(), p[2].get<float>());
        }
        
        if (t.contains("rotation")) {
            const auto& r = t["rotation"];
            // Convert degrees to radians
            entity->rotation = glm::vec3(
                glm::radians(r[0].get<float>()),
                glm::radians(r[1].get<float>()),
                glm::radians(r[2].get<float>())
            );
        }
        
        if (t.contains("scale")) {
            const auto& s = t["scale"];
            if (s.is_array()) {
                entity->scale = glm::vec3(s[0].get<float>(), s[1].get<float>(), s[2].get<float>());
            } else {
                // Uniform scale
                float uniformScale = s.get<float>();
                entity->scale = glm::vec3(uniformScale);
            }
        }
    }
    
    // Load components
    if (entityJson.contains("components")) {
        LoadComponents(entity, entityJson["components"]);
    }
    
    // Load children recursively
    if (entityJson.contains("children")) {
        for (const auto& childJson : entityJson["children"]) {
            LoadEntity(world, childJson, entity);
        }
    }
    
    std::cout << "✓ Created entity: " << name << "\n";
    return entity;
}

void SceneLoader::LoadComponents(Entity* entity, const json& componentsJson) {
    for (const auto& compJson : componentsJson) {
        std::string type = compJson.value("type", "");
        
        if (type == "Camera") {
            LoadCameraComponent(entity, compJson);
        } else if (type == "MeshRenderer") {
            LoadMeshRendererComponent(entity, compJson);
        } else {
            std::cerr << "Warning: Unknown component type: " << type << "\n";
        }
    }
}

void SceneLoader::LoadCameraComponent(Entity* entity, const json& camJson) {
    auto* camera = entity->AddComponent<CameraComponent>();
    
    std::string typeStr = camJson.value("cameraType", "perspective");
    if (typeStr == "orthographic") {
        camera->cameraType = CameraComponent::ORTHOGRAPHIC;
    } else {
        camera->cameraType = CameraComponent::PERSPECTIVE;
    }
    
    camera->fovY = glm::radians(camJson.value("fovY", 60.0f));
    camera->orthoHeight = camJson.value("orthoHeight", 10.0f);
    camera->nearPlane = camJson.value("near", 0.1f);
    camera->farPlane = camJson.value("far", 100.0f);
    
    std::cout << "  + Camera component\n";
}

void SceneLoader::LoadMeshRendererComponent(Entity* entity, const json& rendererJson) {
    auto* renderer = entity->AddComponent<MeshRendererComponent>();
    
    // Load mesh
    if (rendererJson.contains("mesh")) {
        std::string meshName = rendererJson["mesh"];
        Model* model = MeshLoader::Instance().Get(meshName);
        if (model && !model->meshes.empty()) {
            // For now, use first mesh
            // TODO: Support multi-mesh models with mesh index
            int meshIndex = rendererJson.value("meshIndex", 0);
            if (meshIndex >= 0 && meshIndex < (int)model->meshes.size()) {
                renderer->mesh = &model->meshes[meshIndex];
            }
        } else {
            std::cerr << "Warning: Mesh not found: " << meshName << "\n";
        }
    }
    
    // Load material
    if (rendererJson.contains("material")) {
        renderer->material = LoadMaterial(rendererJson["material"]);
    }
    
    std::cout << "  + MeshRenderer component\n";
}

std::unique_ptr<Material> SceneLoader::LoadMaterial(const json& matJson) {
    std::string type = matJson.value("type", "tinted");
    std::unique_ptr<Material> material;
    
    if (type == "textured") {
        auto mat = std::make_unique<TexturedMaterial>();
        
        // Load textures
        if (matJson.contains("albedoMap")) {
            mat->albedoMap = TextureLoader::Instance().Get(matJson["albedoMap"]);
        }
        if (matJson.contains("specularMap")) {
            mat->specularMap = TextureLoader::Instance().Get(matJson["specularMap"]);
        }
        if (matJson.contains("normalMap")) {
            mat->normalMap = TextureLoader::Instance().Get(matJson["normalMap"]);
        }
        if (matJson.contains("emissiveMap")) {
            mat->emissiveMap = TextureLoader::Instance().Get(matJson["emissiveMap"]);
        }
        
        // Load tint
        if (matJson.contains("tint")) {
            const auto& t = matJson["tint"];
            mat->tint = glm::vec4(
                t[0].get<float>(),
                t[1].get<float>(),
                t[2].get<float>(),
                t.size() > 3 ? t[3].get<float>() : 1.0f
            );
        }
        
        // Create sampler
        mat->sampler = std::make_shared<Sampler>();
        
        material = std::move(mat);
    } else {
        // Tinted material
        auto mat = std::make_unique<TintedMaterial>();
        
        if (matJson.contains("tint")) {
            const auto& t = matJson["tint"];
            mat->tint = glm::vec4(
                t[0].get<float>(),
                t[1].get<float>(),
                t[2].get<float>(),
                t.size() > 3 ? t[3].get<float>() : 1.0f
            );
        }
        
        material = std::move(mat);
    }
    
    // Load shader
    if (matJson.contains("shader")) {
        std::string shaderName = matJson["shader"];
        Shader* shader = ShaderLoader::Instance().Get(shaderName);
        if (shader) {
            material->shader = std::shared_ptr<Shader>(shader, [](Shader*) {
                // Empty deleter - ShaderLoader owns the shader
            });
        } else {
            std::cerr << "Warning: Shader not found: " << shaderName << "\n";
        }
    }
    
    // Load pipeline state
    if (matJson.contains("pipelineState")) {
        LoadPipelineState(material->pipelineState, matJson["pipelineState"]);
    }
    
    // Load transparency flag
    material->transparent = matJson.value("transparent", false);
    
    return material;
}

void SceneLoader::LoadPipelineState(PipelineState& state, const json& stateJson) {
    // Face culling
    if (stateJson.contains("faceCulling")) {
        state.faceCulling = stateJson["faceCulling"].get<bool>();
    }
    if (stateJson.contains("cullFace")) {
        std::string cullFace = stateJson["cullFace"];
        if (cullFace == "front") {
            state.cullFace = GL_FRONT;
        } else if (cullFace == "back") {
            state.cullFace = GL_BACK;
        } else if (cullFace == "front_and_back") {
            state.cullFace = GL_FRONT_AND_BACK;
        }
    }
    
    // Depth testing
    if (stateJson.contains("depthTesting")) {
        state.depthTesting = stateJson["depthTesting"].get<bool>();
    }
    if (stateJson.contains("depthMask")) {
        state.depthMask = stateJson["depthMask"].get<bool>();
    }
    
    // Blending
    if (stateJson.contains("blending")) {
        state.blending = stateJson["blending"].get<bool>();
    }
    
    // Color mask
    if (stateJson.contains("colorMask")) {
        const auto& mask = stateJson["colorMask"];
        if (mask.is_array() && mask.size() == 4) {
            state.colorMask[0] = mask[0].get<bool>();
            state.colorMask[1] = mask[1].get<bool>();
            state.colorMask[2] = mask[2].get<bool>();
            state.colorMask[3] = mask[3].get<bool>();
        }
    }
}

} // namespace engine