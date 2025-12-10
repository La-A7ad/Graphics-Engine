#include "Engine/CameraComponent.hpp"
#include "Engine/Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

CameraComponent::CameraComponent()
    : cameraType(PERSPECTIVE),
      fovY(glm::radians(60.0f)),
      orthoHeight(10.0f),
      nearPlane(0.1f),
      farPlane(100.0f),
      aspectRatio(16.0f / 9.0f) {
}

glm::mat4 CameraComponent::GetProjectionMatrix() const {
    if (cameraType == PERSPECTIVE) {
        return glm::perspective(fovY, aspectRatio, nearPlane, farPlane);
    } else {
        float halfWidth = orthoHeight * aspectRatio * 0.5f;
        float halfHeight = orthoHeight * 0.5f;
        return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
    }
}

glm::mat4 CameraComponent::GetViewMatrix() const {
    if (!entity) {
        return glm::mat4(1.0f);
    }
    return glm::inverse(entity->GetWorldTransform());
}

glm::vec3 CameraComponent::GetPosition() const {
    return entity ? entity->GetWorldPosition() : glm::vec3(0);
}

glm::vec3 CameraComponent::GetForward() const {
    return entity ? entity->GetForward() : glm::vec3(0, 0, -1);
}

glm::vec3 CameraComponent::GetRight() const {
    return entity ? entity->GetRight() : glm::vec3(1, 0, 0);
}

glm::vec3 CameraComponent::GetUp() const {
    return entity ? entity->GetUp() : glm::vec3(0, 1, 0);
}

}