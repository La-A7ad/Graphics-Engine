#pragma once

// Single public include for the engine.
// Game code should include only:  #include "Engine/Engine.hpp"

// ---- Core / windowing / rendering ----
#include "Engine/Rendering/Core/Renderer.hpp"

// ---- ECS core ----
#include "Engine/ECS/Core/World/World.hpp"
#include "Engine/ECS/Core/Entity/Entity.hpp"
#include "Engine/ECS/Core/Component/Component.hpp"

// ---- Common components ----
#include "Engine/ECS/Components/Camera/CameraComponent.hpp"
#include "Engine/ECS/Components/Rendering/MeshRendererComponent.hpp"

// ---- Geometry ----
#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include "Engine/Rendering/Geometry/Model/Model.hpp"

// ---- Materials ----
#include "Engine/Rendering/Materials/Base/Material.hpp"
#include "Engine/Rendering/Materials/Implementations/TintedMaterial.hpp"
#include "Engine/Rendering/Materials/Implementations/TexturedMaterial.hpp"

// ---- Graphics objects ----
#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include "Engine/Core/Graphics/Buffers/Buffers.hpp"
#include "Engine/Core/Graphics/Texture/Texture.hpp"
#include "Engine/Core/Graphics/Texture/Sampler.hpp"
#include "Engine/Core/Graphics/State/PipelineState.hpp"

// ---- Asset loaders ----
#include "Engine/Assets/Loaders/Shader/ShaderLoader.hpp"
#include "Engine/Assets/Loaders/Texture/TextureLoader.hpp"
#include "Engine/Assets/Loaders/Mesh/MeshLoader.hpp"

// ---- Scene system ----
#include "Engine/Scene/SceneLoader.hpp"

// ---- Input system ----
#include "Engine/Core/Input/InputManager.hpp"