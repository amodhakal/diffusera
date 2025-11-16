#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;

out vec4 outColor;

void main() {
    vec3 color = fragColor;
    
    if (fragNormal[0] != 0) {
        // Right or left
        color *= .8;
    } else if (fragNormal[1] == 1) {
        // Nothing
    } else if (fragNormal[1] == -1) {
        color *= .7;
    } else {
        color *= .9;
    }

    outColor = vec4(color, 1.0);
}
