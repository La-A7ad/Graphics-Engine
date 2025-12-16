#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec4 uTint;

out vec4 FragColor;

// DEBUG VERSION: prove we're seeing the tint
void main() {
    FragColor = uTint;
}
