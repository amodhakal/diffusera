#version 330 core

const int RIGHT_LEFT_NORMAL = 0;
const int FRONT_BACK_NORMAL = 1;
const int BOTTOM_NORMAL = 2;
const int TOP_NORMAL = 3;

in vec3 fragColor;
flat in int fragNormal;
in vec3 fragPosition;

out vec4 outColor;

void main() {
    vec4 color = vec4(fragColor, 1.0);

    if (fragNormal == RIGHT_LEFT_NORMAL) {
        color.xyz *= 0.8;
    } else if (fragNormal == FRONT_BACK_NORMAL) {
        color.xyz *= 0.9;
    } else if (fragNormal == TOP_NORMAL) {
        color.xyz *= 1.0;
    } else if (fragNormal == BOTTOM_NORMAL) {
        color.xyz *= 0.7;
    } else {
        color = vec4(0.0, 0.0, 0.0, 0.0);
    }

    outColor = color;
}
