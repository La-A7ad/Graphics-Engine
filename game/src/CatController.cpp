#include "CatController.hpp"
#include "Engine/Core/Input/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

CatController::CatController(engine::Entity* catEntity, engine::Entity* cameraEntity)
    : m_CatEntity(catEntity), m_CameraEntity(cameraEntity) {
}

void CatController::Update(float deltaTime) {
    if (!m_CatEntity || !m_CameraEntity) return;
    
    auto& input = engine::InputManager::Instance();
    
    // --- Mouse Rotation (hold right mouse button) ---
    if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        glm::vec2 mouseDelta = input.GetMouseDelta();
        
        if (m_FirstMouse) {
            m_FirstMouse = false;
        } else {
            m_CatYaw += mouseDelta.x * lookSensitivity;
            
            // Update cat rotation (Y-axis only for character rotation)
            m_CatEntity->rotation.y = glm::radians(m_CatYaw);
        }
    } else {
        m_FirstMouse = true;
    }
    
    // --- Keyboard Movement ---
    float speed = moveSpeed;
    if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        speed = sprintSpeed;
    }
    
    glm::vec3 moveDir(0.0f);
    
    // Forward/Backward (in cat's local space)
    if (input.IsKeyPressed(GLFW_KEY_W)) {
        moveDir += m_CatEntity->GetForward();
    }
    if (input.IsKeyPressed(GLFW_KEY_S)) {
        moveDir -= m_CatEntity->GetForward();
    }
    
    // Strafe Left/Right (in cat's local space)
    if (input.IsKeyPressed(GLFW_KEY_A)) {
        moveDir -= m_CatEntity->GetRight();
    }
    if (input.IsKeyPressed(GLFW_KEY_D)) {
        moveDir += m_CatEntity->GetRight();
    }
    
    // Normalize and apply movement (only in XZ plane)
    if (glm::length(moveDir) > 0.01f) {
        moveDir.y = 0.0f; // Keep on ground
        moveDir = glm::normalize(moveDir);
        m_CatEntity->position += moveDir * speed * deltaTime;
    }
    
    // --- Update Camera to Follow Cat ---
    UpdateCameraPosition();
}

void CatController::UpdateCameraPosition() {
    // Calculate desired camera position behind and above the cat
    glm::vec3 catPos = m_CatEntity->position;
    glm::vec3 catForward = m_CatEntity->GetForward();
    
    // Position camera behind the cat
    glm::vec3 targetCameraPos = catPos - catForward * cameraDistance;
    targetCameraPos.y = catPos.y + cameraHeight;
    
    // Smooth camera movement (lerp)
    glm::vec3 currentCameraPos = m_CameraEntity->position;
    m_CameraEntity->position = glm::mix(currentCameraPos, targetCameraPos, cameraSmoothing);
    
    // Make camera look at cat (slightly above cat position)
    glm::vec3 lookAtTarget = catPos + glm::vec3(0.0f, 1.0f, 0.0f); // Look at cat's "head"
    glm::vec3 cameraPos = m_CameraEntity->position;
    
    // Calculate rotation to look at cat
    glm::vec3 direction = glm::normalize(lookAtTarget - cameraPos);
    
    // Calculate pitch (X rotation)
    float pitch = std::asin(-direction.y);
    
    // Calculate yaw (Y rotation)
    float yaw = std::atan2(direction.x, direction.z);
    
    // Apply rotation to camera
    m_CameraEntity->rotation.x = pitch;
    m_CameraEntity->rotation.y = yaw;
    m_CameraEntity->rotation.z = 0.0f;
}