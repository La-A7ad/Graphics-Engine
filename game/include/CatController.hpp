#pragma once

#include "Engine/Engine.hpp"

/**
 * CatController - Third-person character controller for Cat
 * 
 * Controls:
 * - WASD: Move cat (forward/back/strafe)
 * - Mouse: Rotate cat (when right button held)
 * - Shift: Sprint
 * 
 * Camera automatically follows behind the cat
 */
class CatController {
public:
    CatController(engine::Entity* catEntity, engine::Entity* cameraEntity);
    
    void Update(float deltaTime);
    
    // Movement settings
    float moveSpeed = 3.0f;
    float sprintSpeed = 6.0f;
    float rotationSpeed = 2.0f;
    
    // Camera follow settings
    float cameraDistance = 5.0f;
    float cameraHeight = 3.0f;
    float cameraSmoothing = 0.1f;
    float lookSensitivity = 0.1f;
    
private:
    engine::Entity* m_CatEntity;
    engine::Entity* m_CameraEntity;
    
    float m_CatYaw = 0.0f;
    bool m_FirstMouse = true;
    
    void UpdateCameraPosition();
};