cmake_minimum_required(VERSION 3.16)

project(GraphicsEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

add_library(Engine STATIC
    # Core Graphics - Rendering
    engine/src/Rendering/Core/Renderer.cpp
    
    # Core Graphics - Buffers
    engine/src/Core/Graphics/Buffers/Buffers.cpp
    
    # Core Graphics - Shader
    engine/src/Core/Graphics/Shader/Shader.cpp
    
    # Core Graphics - State
    engine/src/Core/Graphics/State/PipelineState.cpp
    
    # Core Graphics - Texture
    engine/src/Core/Graphics/Texture/Texture.cpp
    engine/src/Core/Graphics/Texture/Sampler.cpp
    
    # Core Math
    engine/src/Core/Math/Transform.cpp
    
    # Rendering - Geometry
    engine/src/Rendering/Geometry/Mesh/Mesh.cpp
    engine/src/Rendering/Geometry/Model/Model.cpp
    
    # Rendering - Materials
    engine/src/Rendering/Materials/Base/Material.cpp
    engine/src/Rendering/Materials/Implementations/TintedMaterial.cpp
    engine/src/Rendering/Materials/Implementations/TexturedMaterial.cpp
    
    # ECS - Core
    engine/src/ECS/Core/Entity/Entity.cpp
    engine/src/ECS/Core/World/World.cpp
    
    # ECS - Components
    engine/src/ECS/Components/Camera/CameraComponent.cpp
    engine/src/ECS/Components/Rendering/MeshRendererComponent.cpp
    
    # Assets - Loaders
    engine/src/Assets/Loaders/Shader/ShaderLoader.cpp
    engine/src/Assets/Loaders/Texture/TextureLoader.cpp
    engine/src/Assets/Loaders/Mesh/MeshLoader.cpp
    
    # External
    external/glad/src/glad.c
)

target_include_directories(Engine PUBLIC
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/tinyobjloader
    ${CMAKE_SOURCE_DIR}/external/stb
)

target_link_libraries(Engine PUBLIC
    OpenGL::GL
    glfw
)

add_executable(GameApp
    game/main.cpp
)

target_link_libraries(GameApp PRIVATE Engine)

add_custom_command(
    TARGET GameApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/game/assets
        ${CMAKE_CURRENT_BINARY_DIR}/game/assets
)