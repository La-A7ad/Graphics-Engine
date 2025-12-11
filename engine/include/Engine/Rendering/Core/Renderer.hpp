#pragma once

#include "Engine/Shader.hpp"
#include "Engine/Buffers.hpp"
#include <memory>
#include <glm/glm.hpp>

struct GLFWwindow;

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
    GLFWwindow* GetWindow() const { return m_Window; }
    
private:
    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;
    int width;
    int height;
    bool initialized;
    GLFWwindow* m_Window = nullptr;
    
    void RenderEntity(MeshRendererComponent* renderer, const glm::mat4& view, const glm::mat4& proj);
};

}