cmake_minimum_required(VERSION 3.10)
project(FloatingGardenEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(glad STATIC libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)


set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(libs/glfw)


set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(libs/assimp)   


file(GLOB SRC_FILES 
    src/*.cpp 
    src/*.c 
    src/model/*.cpp
)


add_executable(Engine ${SRC_FILES})


target_include_directories(Engine PRIVATE
    include
    libs/glm
    libs/glad/include
    libs/assimp/include        
)


target_link_libraries(Engine
    glad
    glfw
    assimp                     
    opengl32
)