#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

void main() {
    vec3 lightPosition = vec3(0.0, 0.0, 0.5);
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * fragColor;

    outColor = vec4(diffuse, 1.0);
}
