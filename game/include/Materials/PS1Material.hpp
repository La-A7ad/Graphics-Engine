// game/include/Materials/PS1Material.hpp
#pragma once

#include "Engine/Engine.hpp"
#include <glm/glm.hpp>

/**
 * PS1Material - Game-specific material for PlayStation 1 era visual effects
 * 
 * Extends the engine's TexturedMaterial with PS1-style rendering:
 * - Vertex snapping (wobbly geometry)
 * - Affine texture mapping (texture warping)
 * - Color depth reduction (color banding)
 * - Ordered dithering
 * - Distance fog
 */
class PS1Material : public engine::TexturedMaterial {
public:
    // === VERTEX SNAPPING ===
    glm::vec2 snapResolution = glm::vec2(320.0f, 240.0f);  // PS1 screen resolution
    float snapStrength = 1.0f;  // 0.0 = off, 1.0 = full PS1 wobble
    
    // === COLOR QUANTIZATION ===
    float colorDepth = 32.0f;  // 32 = PS1 (5-bit), 64 = smoother, 256 = disabled
    
    // === DITHERING ===
    float ditherStrength = 1.0f;  // 0.0 = off, 1.0 = full dither pattern
    
    // === DISTANCE FOG ===
    float fogStart = 20.0f;
    float fogEnd = 50.0f;
    glm::vec3 fogColor = glm::vec3(0.5f, 0.5f, 0.6f);
    
    PS1Material();
    
    void Setup() override;
    
    // Preset configurations
    void SetAuthenticPS1();     // Maximum PS1 accuracy (very wobbly)
    void SetPS1Inspired();      // Subtle, more playable
    void SetAffineOnly();       // Just texture warping, no other effects
};