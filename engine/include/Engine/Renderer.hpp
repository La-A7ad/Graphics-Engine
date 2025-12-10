#pragma once

#include "Engine/Shader.hpp"
#include "Engine/Buffers.hpp"

namespace engine {

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Init();
    void RenderFrame(float time);
    void Resize(int w, int h);
    
private:
    VAO vao;
    VBO vbo;
    Shader* shader;
    
    int width;
    int height;
    bool initialized;
};

} // namespace engine