#pragma once

#include "Engine/Engine.hpp"

/**
 * CameraController - Simple FPS-style camera controller
 * 
 * Controls:
 * - WASD: Move camera
 * - Mouse: Look around (when right button held)
 * - Q/E: Move up/down
 * - Shift: Move faster
 */
class CameraController {
public:
    CameraController(engine::Entity* cameraEntity);
    
    void Update(float deltaTime);
    
    float moveSpeed = 3.0f;
    float sprintSpeed = 6.0f;
    float lookSensitivity = 0.1f;
    float verticalSpeed = 2.0f;
    
private:
    engine::Entity* m_CameraEntity;
    float m_Yaw = -90.0f;   // Start looking forward (-Z)
    float m_Pitch = 0.0f;
    bool m_FirstMouse = true;
};