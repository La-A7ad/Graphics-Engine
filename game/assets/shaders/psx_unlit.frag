#version 330 core

noperspective in vec2 vTexCoords;
noperspective in float vDepth;
in vec3 vColor;

uniform sampler2D uAlbedoMap;
uniform vec4 uTint;

// PS1 effect toggles
uniform float uColorDepth;    // 32.0 for PS1 (5-bit per channel), 256.0 to disable
uniform float uDitherStrength; // 0.0 to 1.0
uniform float uFogStart;      // Fog distance parameters
uniform float uFogEnd;
uniform vec3 uFogColor;

out vec4 FragColor;

// Bayer matrix for ordered dithering (PS1 style)
float dither8x8(vec2 position, float brightness) {
    int x = int(mod(position.x, 8.0));
    int y = int(mod(position.y, 8.0));
    
    // 8x8 Bayer matrix
    int index = x + y * 8;
    float limit = 0.0;
    
    if (index == 0) limit = 0.0;
    else if (index == 1) limit = 32.0;
    else if (index == 2) limit = 8.0;
    else if (index == 3) limit = 40.0;
    else if (index == 4) limit = 2.0;
    else if (index == 5) limit = 34.0;
    else if (index == 6) limit = 10.0;
    else if (index == 7) limit = 42.0;
    else if (index == 8) limit = 48.0;
    else if (index == 9) limit = 16.0;
    else if (index == 10) limit = 56.0;
    else if (index == 11) limit = 24.0;
    else if (index == 12) limit = 50.0;
    else if (index == 13) limit = 18.0;
    else if (index == 14) limit = 58.0;
    else if (index == 15) limit = 26.0;
    else if (index == 16) limit = 12.0;
    else if (index == 17) limit = 44.0;
    else if (index == 18) limit = 4.0;
    else if (index == 19) limit = 36.0;
    else if (index == 20) limit = 14.0;
    else if (index == 21) limit = 46.0;
    else if (index == 22) limit = 6.0;
    else if (index == 23) limit = 38.0;
    else if (index == 24) limit = 60.0;
    else if (index == 25) limit = 28.0;
    else if (index == 26) limit = 52.0;
    else if (index == 27) limit = 20.0;
    else if (index == 28) limit = 62.0;
    else if (index == 29) limit = 30.0;
    else if (index == 30) limit = 54.0;
    else if (index == 31) limit = 22.0;
    else if (index == 32) limit = 3.0;
    else if (index == 33) limit = 35.0;
    else if (index == 34) limit = 11.0;
    else if (index == 35) limit = 43.0;
    else if (index == 36) limit = 1.0;
    else if (index == 37) limit = 33.0;
    else if (index == 38) limit = 9.0;
    else if (index == 39) limit = 41.0;
    else if (index == 40) limit = 51.0;
    else if (index == 41) limit = 19.0;
    else if (index == 42) limit = 59.0;
    else if (index == 43) limit = 27.0;
    else if (index == 44) limit = 49.0;
    else if (index == 45) limit = 17.0;
    else if (index == 46) limit = 57.0;
    else if (index == 47) limit = 25.0;
    else if (index == 48) limit = 15.0;
    else if (index == 49) limit = 47.0;
    else if (index == 50) limit = 7.0;
    else if (index == 51) limit = 39.0;
    else if (index == 52) limit = 13.0;
    else if (index == 53) limit = 45.0;
    else if (index == 54) limit = 5.0;
    else if (index == 55) limit = 37.0;
    else if (index == 56) limit = 63.0;
    else if (index == 57) limit = 31.0;
    else if (index == 58) limit = 55.0;
    else if (index == 59) limit = 23.0;
    else if (index == 60) limit = 61.0;
    else if (index == 61) limit = 29.0;
    else if (index == 62) limit = 53.0;
    else limit = 21.0;
    
    return brightness < limit / 64.0 ? 0.0 : 1.0;
}

void main() {
    // === AFFINE TEXTURE MAPPING ===
    // Sample texture with affine UVs (warps with distance, classic PS1 look)
    vec4 texColor = texture(uAlbedoMap, vTexCoords);
    
    // === APPLY VERTEX LIGHTING ===
    vec3 litColor = texColor.rgb * vColor;
    
    // === COLOR DEPTH REDUCTION (PS1 = 5-bit per channel = 32 levels) ===
    litColor = floor(litColor * uColorDepth) / uColorDepth;
    
    // === DITHERING (PS1 used this to fake more colors) ===
    if (uDitherStrength > 0.0) {
        float brightness = dot(litColor, vec3(0.299, 0.587, 0.114));
        float dither = dither8x8(gl_FragCoord.xy, brightness);
        litColor += (dither - 0.5) * (uDitherStrength / uColorDepth);
        litColor = clamp(litColor, 0.0, 1.0);
    }
    
    // === DISTANCE FOG (optional PS1 effect) ===
    float depth = abs(vDepth);
    float fogFactor = smoothstep(uFogStart, uFogEnd, depth);
    litColor = mix(litColor, uFogColor, fogFactor);
    
    // Apply tint and output
    FragColor = vec4(litColor, texColor.a * uTint.a);
}