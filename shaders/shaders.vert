#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

out vec3 fragColor;
out vec3 fragNormal;

uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragNormal = inNormal;
}
