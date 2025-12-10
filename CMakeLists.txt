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
    # Core rendering
    engine/src/Shader.cpp
    engine/src/Buffers.cpp
    engine/src/Mesh.cpp
    engine/src/Model.cpp
    engine/src/Renderer.cpp
    
    # ECS
    engine/src/Entity.cpp
    engine/src/World.cpp
    
    # Components
    engine/src/CameraComponent.cpp
    engine/src/MeshRendererComponent.cpp
    
    # Materials
    engine/src/Material.cpp
    engine/src/TintedMaterial.cpp
    engine/src/TexturedMaterial.cpp
    engine/src/PipelineState.cpp
    
    # Assets
    engine/src/Texture.cpp
    engine/src/Sampler.cpp
    
    # Asset Loaders
    engine/src/ShaderLoader.cpp
    engine/src/TextureLoader.cpp
    engine/src/MeshLoader.cpp
    
    # External
    external/glad/src/glad.c
    
    # REMOVED: engine/src/Transform.cpp (duplicate/useless)
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