#pragma once
#include "shader.h"
#include <glad/glad.h>

class Renderer {
public:
    Renderer() = default;
    ~Renderer();
    bool init(); 
    void renderFrame(float time); 
    void resize(int w, int h) { width = w; height = h; glViewport(0,0,w,h); }
private:
    GLuint VAO = 0, VBO = 0;
    Shader shader;
    int width = 800, height = 600;
    bool initialized = false;
    void cleanup();
};
