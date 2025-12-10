#version 330 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec4 uTint;

out vec4 FragColor;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    
    vec3 ambient = vec3(0.3);
    vec3 result = (ambient + diffuse) * uTint.rgb;
    
    FragColor = vec4(result, uTint.a);
}