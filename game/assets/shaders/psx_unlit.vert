#version 330 core
layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

uniform vec2  uSnapRes;       // set from code; if (0,0) -> snapping disabled
uniform float uSnapStrength;  // 0..1

noperspective out vec2 vTexCoords;
out vec3 vViewPos;

void main() {
    vec4 viewPos = uView * (uModel * vec4(aPosition, 1.0));
    vec4 clipPos = uProj * viewPos;

    vec3 ndc = clipPos.xyz / clipPos.w;

    // Safe snapping: only if res is valid and strength > 0
    if (uSnapStrength > 0.0 && uSnapRes.x > 1.0 && uSnapRes.y > 1.0) {
        vec2 snapped = floor(ndc.xy * uSnapRes) / uSnapRes;
        ndc.xy = mix(ndc.xy, snapped, clamp(uSnapStrength, 0.0, 1.0));
    }

    gl_Position = vec4(ndc * clipPos.w, clipPos.w);

    vTexCoords = aTexCoords;
    vViewPos   = viewPos.xyz;
}
