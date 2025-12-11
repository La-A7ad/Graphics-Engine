cmake_minimum_required(VERSION 3.10)


# ===================================
# Engine library
# ===================================

add_library(engine
    # Assets / Loaders
    engine/src/Assets/Loaders/Mesh/MeshLoader.cpp
    engine/src/Assets/Loaders/Shader/ShaderLoader.cpp
    engine/src/Assets/Loaders/Texture/TextureLoader.cpp

    # Core / Graphics / Buffers
    engine/src/Core/Graphics/Buffers/Buffers.cpp

    # Core / Graphics / Shader
    engine/src/Core/Graphics/Shader/Shader.cpp

    # Core / Graphics / State
    engine/src/Core/Graphics/State/PipelineState.cpp

    # Core / Graphics / Texture
    engine/src/Core/Graphics/Texture/Sampler.cpp
    engine/src/Core/Graphics/Texture/STBImageImpl.cpp      # STB implementation
    engine/src/Core/Graphics/Texture/Texture.cpp

    # Core / Math
    engine/src/Core/Math/Transform.cpp

    # ECS / Components
    engine/src/ECS/Components/Camera/CameraComponent.cpp
    engine/src/ECS/Components/Rendering/MeshRendererComponent.cpp

    # ECS / Core
    engine/src/ECS/Core/Entity/Entity.cpp
    engine/src/ECS/Core/World/World.cpp

    # Rendering / Core
    engine/src/Rendering/Core/Renderer.cpp

    # Rendering / Geometry
    engine/src/Rendering/Geometry/Mesh/Mesh.cpp
    engine/src/Rendering/Geometry/Model/Model.cpp

    # Rendering / Materials
    engine/src/Rendering/Materials/Base/Material.cpp
    engine/src/Rendering/Materials/Implementations/TexturedMaterial.cpp
    engine/src/Rendering/Materials/Implementations/TintedMaterial.cpp
)

# Public include directories
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader
)

# Link external libraries
target_link_libraries(engine PUBLIC
    glad
    glm
    glfw
)

# C++ standard
set_target_properties(engine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)