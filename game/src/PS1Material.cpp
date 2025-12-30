// game/src/Materials/PS1Material.cpp
#include "Materials/PS1Material.hpp"
#include <iostream>

PS1Material::PS1Material() {
    // Default to authentic PS1 settings
    SetAuthenticPS1();
    
    // Setup nearest-neighbor filtering (critical for PS1 look!)
    sampler = std::make_shared<engine::Sampler>();
    sampler->SetMinFilter(GL_NEAREST);
    sampler->SetMagFilter(GL_NEAREST);
}

void PS1Material::Setup() {
    // First setup textures and base material properties
    engine::TexturedMaterial::Setup();
    
    if (!shader) {
        std::cerr << "Warning: PS1Material::Setup called with null shader\n";
        return;
    }
    
    shader->use();
    
    // === VERTEX SNAPPING ===
    shader->setVec2("uSnapRes", snapResolution);
    shader->setFloat("uSnapStrength", snapStrength);
    
    // === COLOR DEPTH REDUCTION ===
    shader->setFloat("uColorDepth", colorDepth);
    
    // === DITHERING ===
    shader->setFloat("uDitherStrength", ditherStrength);
    
    // === DISTANCE FOG ===
    shader->setFloat("uFogStart", fogStart);
    shader->setFloat("uFogEnd", fogEnd);
    shader->setVec3("uFogColor", fogColor);
}

void PS1Material::SetAuthenticPS1() {
    // Maximum PS1 authenticity - very wobbly & high color banding
    snapResolution = glm::vec2(320.0f, 240.0f);  // Original PS1 resolution
    snapStrength = 1.0f;                         // Full wobble
    colorDepth = 32.0f;                          // 5-bit color (authentic)
    ditherStrength = 1.0f;                       // Full dither
    fogStart = 15.0f;
    fogEnd = 30.0f;
    fogColor = glm::vec3(0.5f, 0.5f, 0.6f);
}

void PS1Material::SetPS1Inspired() {
    // "PS1-inspired" look - more subtle and playable
    snapResolution = glm::vec2(640.0f, 480.0f);  // Less wobbly
    snapStrength = 0.7f;                         // Subtle wobble
    colorDepth = 64.0f;                          // Smoother colors (6-bit)
    ditherStrength = 1.0f;                       // Subtle dither
    fogStart = 20.0f;
    fogEnd = 50.0f;
    fogColor = glm::vec3(0.5f, 0.5f, 0.6f);
}

void PS1Material::SetAffineOnly() {
    // Just affine texture mapping (texture warping only, no other effects)
    snapResolution = glm::vec2(0.0f, 0.0f);      // Disable snapping
    snapStrength = 0.0f;
    colorDepth = 256.0f;                         // Disable color reduction
    ditherStrength = 0.0f;                       // Disable dithering
    fogStart = 1000.0f;                          // Effectively disable fog
    fogEnd = 2000.0f;
    fogColor = glm::vec3(0.0f);
    
    // Still keep nearest-neighbor filtering for pixelated textures
    if (sampler) {
        sampler->SetMinFilter(GL_NEAREST);
        sampler->SetMagFilter(GL_NEAREST);
    }
}