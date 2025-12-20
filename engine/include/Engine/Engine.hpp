#pragma once

// =============================================================
// Engine umbrella header
//
// Game-side usage:
//   #include "Engine/Engine.hpp"
//
// Engine-side headers should generally include what they use,
// not this umbrella.
// =============================================================

// ---- ECS core ----
#include "Engine/ECS/Core/World/World.hpp"
#include "Engine/ECS/Core/Entity/Entity.hpp"
#include "Engine/ECS/Core/Component/Component.hpp"

// ---- ECS components ----
#include "Engine/ECS/Components/Camera/CameraComponent.hpp"
#include "Engine/ECS/Components/Rendering/MeshRendererComponent.hpp"

// ---- Math ----
#include "Engine/Core/Math/Transform.hpp"

// ---- Core graphics wrappers ----
#include "Engine/Core/Graphics/Shader/Shader.hpp"
#include "Engine/Core/Graphics/Buffers/Buffers.hpp"
#include "Engine/Core/Graphics/Texture/Texture.hpp"
#include "Engine/Core/Graphics/Texture/Sampler.hpp"
#include "Engine/Core/Graphics/State/PipelineState.hpp"

// ---- Rendering ----
#include "Engine/Rendering/Core/Renderer.hpp"
#include "Engine/Rendering/Geometry/Mesh/Mesh.hpp"
#include "Engine/Rendering/Geometry/Model/Model.hpp"
#include "Engine/Rendering/Materials/Base/Material.hpp"
#include "Engine/Rendering/Materials/Implementations/TintedMaterial.hpp"
#include "Engine/Rendering/Materials/Implementations/TexturedMaterial.hpp"

// ---- Asset loaders ----
#include "Engine/Assets/Loaders/Mesh/MeshLoader.hpp"
#include "Engine/Assets/Loaders/Texture/TextureLoader.hpp"
#include "Engine/Assets/Loaders/Shader/ShaderLoader.hpp"
