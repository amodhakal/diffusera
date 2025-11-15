#pragma once
#include <sys/types.h>

#include <glm/glm.hpp>

namespace Constants {
constexpr auto VERTEX_PATH = "./shaders/shaders.vert";
constexpr auto FRAGMENT_PATH = "./shaders/shaders.frag";

constexpr uint SCR_WIDTH = 800;
constexpr uint SCR_HEIGHT = 600;

constexpr float CAMERA_SPEED = 1.5;
constexpr float CAMERA_SENSITIVITY = 0.1;

constexpr auto BG_COLOR = glm::vec4(0.1, 0.1, 0.1, 1.0);
}  // namespace Constants
