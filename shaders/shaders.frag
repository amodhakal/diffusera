#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
out vec4 outColor;

void main() {
    outColor = vec4(fragNormal[2] > 0.0 ? fragColor : vec3(0.0, 0.0, 0.0), 1.0);
}
