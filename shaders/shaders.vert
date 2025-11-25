#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inNormal;
layout(location = 2) in vec3 inColor;

out vec3 fragColor;
flat out int fragNormal;
out vec3 fragPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
    fragPosition = inPosition;
    fragColor = inColor;
    fragNormal = int(inNormal);
}
