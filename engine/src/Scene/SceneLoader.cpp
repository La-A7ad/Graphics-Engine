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
        LoadAssets(sceneJson["assets"], sceneDir);
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
        LoadComponents(entity, entityJson["components"], world);
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

void SceneLoader::LoadComponents(Entity* entity, const json& componentsJson, World* world) {
    for (const auto& compJson : componentsJson) {
        std::string type = compJson.value("type", "");
        
        if (type == "Camera") {
            LoadCameraComponent(entity, compJson);
        } else if (type == "MeshRenderer") {
            LoadMeshRendererComponent(entity, compJson, world);
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

// Helper to create a material from embedded material data
std::unique_ptr<Material> SceneLoader::CreateMaterialFromEmbedded(
    const EmbeddedMaterialData& matData, 
    const std::string& modelDirectory,
    Shader* defaultShader) 
{
    auto mat = std::make_unique<TexturedMaterial>();
    
    auto& texLoader = TextureLoader::Instance();
    
    // Load base color texture if available
    if (!matData.baseColorTexturePath.empty()) {
        std::string fullPath = modelDirectory + "/" + matData.baseColorTexturePath;
        // Use the texture path as a unique name (prefixed to avoid collisions)
        std::string texName = "embedded_" + matData.baseColorTexturePath;
        
        Texture* tex = texLoader.Get(texName);
        if (!tex) {
            tex = texLoader.Load(texName, fullPath);
        }
        mat->albedoMap = tex;
    }
    
    // Set tint from base color factor
    mat->tint = glm::vec4(matData.baseColorR, matData.baseColorG, matData.baseColorB, matData.baseColorA);
    
    // Create sampler
    mat->sampler = std::make_shared<Sampler>();
    
    // Set shader
    if (defaultShader) {
        mat->shader = std::shared_ptr<Shader>(defaultShader, [](Shader*) {
            // Empty deleter - ShaderLoader owns the shader
        });
    }
    
    // Configure pipeline state based on material properties
    if (matData.doubleSided) {
        mat->pipelineState.faceCulling = false;
    } else {
        mat->pipelineState.faceCulling = true;
        mat->pipelineState.cullFace = GL_BACK;
    }
    
    // Handle alpha modes
    if (matData.alphaMode == "BLEND") {
        mat->transparent = true;
        mat->pipelineState.blending = true;
    } else if (matData.alphaMode == "MASK") {
        // For alpha cutoff, we'd need shader support
        // For now, treat as opaque
        mat->transparent = false;
    } else {
        mat->transparent = false;
    }
    
    mat->pipelineState.depthTesting = true;
    mat->pipelineState.depthMask = !mat->transparent;
    
    return mat;
}

void SceneLoader::LoadMeshRendererComponent(Entity* entity, const json& rendererJson, World* world) {
    // Check if we should use embedded materials
    bool useEmbeddedMaterials = rendererJson.value("useEmbeddedMaterials", false);
    
    std::string meshName;
    if (rendererJson.contains("mesh")) {
        meshName = rendererJson["mesh"];
    }
    
    Model* model = MeshLoader::Instance().Get(meshName);
    if (!model || model->meshes.empty()) {
        std::cerr << "Warning: Mesh not found or empty: " << meshName << "\n";
        return;
    }
    
    // Get default shader for embedded materials
    Shader* defaultShader = ShaderLoader::Instance().Get("textured");
    if (!defaultShader) {
        // Try to load textured shader if not already loaded
        // This is a fallback - normally shaders should be declared in the scene
        std::cerr << "Warning: 'textured' shader not found for embedded materials\n";
    }
    
    if (useEmbeddedMaterials && model->meshes.size() > 1) {
        // Multi-mesh model with embedded materials:
        // Create child entities for each mesh
        std::cout << "  Loading " << model->meshes.size() << " submeshes with embedded materials\n";
        
        for (size_t i = 0; i < model->meshes.size(); ++i) {
            // Create child entity for each submesh
            std::string childName = entity->name + "_submesh_" + std::to_string(i);
            Entity* childEntity = world->CreateEntity(childName);
            entity->AddChild(childEntity);
            
            // Child inherits parent transform (position at origin relative to parent)
            childEntity->position = glm::vec3(0.0f);
            childEntity->rotation = glm::vec3(0.0f);
            childEntity->scale = glm::vec3(1.0f);
            
            // Add MeshRenderer to child
            auto* renderer = childEntity->AddComponent<MeshRendererComponent>();
            renderer->mesh = &model->meshes[i];
            
            // Create material from embedded data
            if (i < model->embeddedMaterials.size()) {
                renderer->material = CreateMaterialFromEmbedded(
                    model->embeddedMaterials[i], 
                    model->directory,
                    defaultShader
                );
            } else {
                // Fallback: create a default tinted material
                auto mat = std::make_unique<TintedMaterial>();
                mat->tint = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
                if (defaultShader) {
                    mat->shader = std::shared_ptr<Shader>(defaultShader, [](Shader*) {});
                }
                renderer->material = std::move(mat);
            }
        }
        
        std::cout << "  + MeshRenderer with " << model->meshes.size() << " submesh entities\n";
        
    } else if (useEmbeddedMaterials && model->meshes.size() == 1) {
        // Single mesh with embedded material
        auto* renderer = entity->AddComponent<MeshRendererComponent>();
        renderer->mesh = &model->meshes[0];
        
        if (!model->embeddedMaterials.empty()) {
            renderer->material = CreateMaterialFromEmbedded(
                model->embeddedMaterials[0],
                model->directory,
                defaultShader
            );
        }
        
        std::cout << "  + MeshRenderer with embedded material\n";
        
    } else {
        // Original behavior: use specified meshIndex and explicit material
        auto* renderer = entity->AddComponent<MeshRendererComponent>();
        
        int meshIndex = rendererJson.value("meshIndex", 0);
        if (meshIndex >= 0 && meshIndex < (int)model->meshes.size()) {
            renderer->mesh = &model->meshes[meshIndex];
        }
        
        // Load explicit material if specified
        if (rendererJson.contains("material")) {
            renderer->material = LoadMaterial(rendererJson["material"]);
        }
        
        std::cout << "  + MeshRenderer component\n";
    }
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