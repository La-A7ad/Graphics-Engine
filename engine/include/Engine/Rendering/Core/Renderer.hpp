#pragma once

#include "Engine/Shader.hpp"
#include "Engine/Buffers.hpp"
#include <glm/glm.hpp>

namespace engine {

class World;
class CameraComponent;
class MeshRendererComponent;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Init();
    void RenderFrame(float time);
    void Render(World* world, CameraComponent* camera);
    void Resize(int w, int h);
    
private:
    VAO vao;
    VBO vbo;
    Shader* shader;
    
    int width;
    int height;
    bool initialized;
    
    void RenderEntity(MeshRendererComponent* renderer, const glm::mat4& view, const glm::mat4& proj);
};

}