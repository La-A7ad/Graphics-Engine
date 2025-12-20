#pragma once

#include "Engine/ECS/Core/Component/Component.hpp"
#include <glm/glm.hpp>

namespace engine {

class CameraComponent : public Component {
public:
    enum Type {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };
    
    Type cameraType;
    
    float fovY;
    float orthoHeight;
    
    float nearPlane;
    float farPlane;
    float aspectRatio;
    
    CameraComponent();
    
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;
    
    glm::vec3 GetPosition() const;
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
};

}