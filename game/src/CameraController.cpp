#include "CameraController.hpp"
#include "Engine/Core/Input/InputManager.hpp"
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <cmath>

CameraController::CameraController(engine::Entity* cameraEntity)
    : m_CameraEntity(cameraEntity) {
}

void CameraController::Update(float deltaTime) {
    if (!m_CameraEntity) return;
    
    auto& input = engine::InputManager::Instance();
    
    // --- Mouse Look (hold right mouse button) ---
    if (input.IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        glm::vec2 mouseDelta = input.GetMouseDelta();
        
        if (m_FirstMouse) {
            m_FirstMouse = false;
        } else {
            m_Yaw += mouseDelta.x * lookSensitivity;
            m_Pitch -= mouseDelta.y * lookSensitivity; // Inverted Y
            
            // Clamp pitch
            if (m_Pitch > 89.0f) m_Pitch = 89.0f;
            if (m_Pitch < -89.0f) m_Pitch = -89.0f;
            
            // Update entity rotation (in radians)
            m_CameraEntity->rotation.y = glm::radians(m_Yaw);
            m_CameraEntity->rotation.x = glm::radians(m_Pitch);
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
    
    // Forward/Backward
    if (input.IsKeyPressed(GLFW_KEY_W)) {
        moveDir += m_CameraEntity->GetForward();
    }
    if (input.IsKeyPressed(GLFW_KEY_S)) {
        moveDir -= m_CameraEntity->GetForward();
    }
    
    // Left/Right
    if (input.IsKeyPressed(GLFW_KEY_A)) {
        moveDir -= m_CameraEntity->GetRight();
    }
    if (input.IsKeyPressed(GLFW_KEY_D)) {
        moveDir += m_CameraEntity->GetRight();
    }
    
    // Up/Down
    if (input.IsKeyPressed(GLFW_KEY_Q)) {
        moveDir.y -= 1.0f;
    }
    if (input.IsKeyPressed(GLFW_KEY_E)) {
        moveDir.y += 1.0f;
    }
    
    // Normalize and apply movement
    if (glm::length(moveDir) > 0.01f) {
        moveDir = glm::normalize(moveDir);
        m_CameraEntity->position += moveDir * speed * deltaTime;
    }
}