cmake_minimum_required(VERSION 3.16)

project(GraphicsEngine LANGUAGES CXX)

# --- C++ standard ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Warnings ---
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Include directories ---
# These paths must match your actual folder layout:
# GraphicsEngine/
#   external/glad/include
#   external/glm
#   engine/include  (it's okay if it's empty for now)
include_directories(
    external/glad/include
    external/glm
    engine/include
)

# --- System libraries: OpenGL + GLFW ---
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# --- Game executable (bootstrap app) ---
add_executable(GameApp
    game/main.cpp
    external/glad/src/glad.c
)

# --- Link everything needed to talk to the GPU ---
target_link_libraries(GameApp
    glfw
    OpenGL::GL
)
