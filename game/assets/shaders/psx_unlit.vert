#version 330 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform vec2  uSnapRes;       // e.g., (320, 240) for PS1 resolution
uniform float uSnapStrength;  // 0..1

// Affine texture mapping (PS1's signature look)
noperspective out vec2 vTexCoords;
noperspective out float vDepth;

// Vertex lighting (PS1 didn't do per-pixel lighting)
out vec3 vColor;

void main() {
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vec4 viewPos = uView * worldPos;
    vec4 clipPos = uProj * viewPos;
    
    // === VERTEX SNAPPING (PS1's wobbly vertices) ===
    vec3 ndc = clipPos.xyz / clipPos.w;
    if (uSnapStrength > 0.0 && uSnapRes.x > 1.0 && uSnapRes.y > 1.0) {
        vec2 snapped = floor(ndc.xy * uSnapRes) / uSnapRes;
        ndc.xy = mix(ndc.xy, snapped, clamp(uSnapStrength, 0.0, 1.0));
    }
    
    gl_Position = vec4(ndc * clipPos.w, clipPos.w);
    
    // === AFFINE TEXTURE MAPPING (no perspective correction) ===
    // Store un-corrected UVs and depth for manual interpolation
    vTexCoords = aTexCoords;
    vDepth = clipPos.w;
    
    // === VERTEX LIGHTING (simple directional light) ===
    vec3 normal = normalize(mat3(uModel) * aNormal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    vec3 ambient = vec3(0.3);
    
    vColor = ambient + diffuse;
}