cmake_minimum_required(VERSION 3.10)
project(GraphicsEngine)

# ===================================
# 1. Build GLAD from Source
# ===================================
# We create a static library 'glad' from the source file.
# CMake will now know that 'glad' is a target, not a system library.
add_library(glad STATIC external/glad/src/glad.c)

# Add the include directories for GLAD (public so linking targets see them)
target_include_directories(glad PUBLIC external/glad/include)

# On Linux, GLAD often needs 'dl' (dynamic linking loader)
if(UNIX)
    target_link_libraries(glad PRIVATE dl)
endif()

# ===================================
# 2. Engine Library
# ===================================
add_library(engine
    #Assets / Importers
    engine/src/Assets/Importers/ObjImporter.cpp
    engine/src/Assets/Importers/GltfImporter.cpp
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
    engine/src/Core/Graphics/Texture/STBImageImpl.cpp
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

    #Core input
    engine/src/Core/Input/InputManager.cpp

    #Scene system
    engine/src/Scene/SceneLoader.cpp
)

# Include Directories
target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/stb
    ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyobjloader
    ${CMAKE_CURRENT_SOURCE_DIR}/external/cgltf
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann/json/single_include

)

# Link Libraries
# Now 'glad' refers to the CMake target defined above, avoiding the linker error.
target_link_libraries(engine PUBLIC
    glad
    glfw
)

# C++ Standard
set_target_properties(engine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ===================================
# 3. GameApp Executable
# ===================================
add_executable(GameApp
    game/main.cpp
    game/src/PS1Material.cpp
    game/src/CameraController.cpp
    game/src/CatController.cpp
)

# Link Engine
target_link_libraries(GameApp PRIVATE engine)

target_include_directories(GameApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/game/include 
)


# Copy Assets
add_custom_command(
    TARGET GameApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game/assets
        ${CMAKE_CURRENT_BINARY_DIR}/game/assets
    COMMENT "Copying game assets to build directory..."
)