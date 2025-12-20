#version 330 core
noperspective in vec2 vTexCoords;
in vec3 vViewPos;

out vec4 FragColor;

uniform sampler2D uAlbedoMap;
uniform vec4 uTint;

// Optional: if you don't set these from code, they default to 0, so use fallbacks
uniform float uColorSteps;        // 0 -> fallback to 31
uniform float uDitherStrength;    // 0..1
uniform vec2  uViewportSize;      // (0,0) -> dithering disabled

float bayer4x4(vec2 p) {
    int x = int(mod(p.x, 4.0));
    int y = int(mod(p.y, 4.0));
    int idx = x + y * 4;
    int m[16] = int[16](
         0,  8,  2, 10,
        12,  4, 14,  6,
         3, 11,  1,  9,
        15,  7, 13,  5
    );
    return float(m[idx]) / 15.0;
}

vec3 posterize(vec3 c, float steps) {
    return floor(c * steps + 0.5) / steps;
}

void main() {
    vec4 texel = texture(uAlbedoMap, vTexCoords);
    vec4 col = texel * uTint;

    float steps = (uColorSteps > 0.0) ? uColorSteps : 31.0;

    if (uDitherStrength > 0.0 && steps > 0.0 && uViewportSize.x > 0.0) {
        float d = bayer4x4(gl_FragCoord.xy);
        float amp = (1.0 / steps) * uDitherStrength;
        col.rgb += (d - 0.5) * amp;
    }

    col.rgb = clamp(col.rgb, 0.0, 1.0);
    col.rgb = posterize(col.rgb, steps);

    FragColor = col;
}
