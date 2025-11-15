#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
