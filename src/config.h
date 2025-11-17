#pragma once
#include <sys/types.h>

#include <glm/glm.hpp>

namespace Constants {
constexpr auto VERTEX_PATH = "./shaders/shaders.vert";
constexpr auto FRAGMENT_PATH = "./shaders/shaders.frag";

constexpr uint SCR_WIDTH = 800;
constexpr uint SCR_HEIGHT = 600;

constexpr auto BG_COLOR = glm::vec4(0.1, 0.1, 0.1, 1.0);

namespace Camera {
constexpr float SPEED = 7.5;
constexpr float SENSITIVITY = 0.2;
constexpr float NEAR = 0.1;
constexpr float FAR = 1000.0;

constexpr float DEFAULT_FOV = 45.0;
constexpr float DEFAULT_YAW = -90.0;
constexpr float DEAULT_PITCH = 0.0;

constexpr float PITCH_MAX = 89.0;
constexpr float PITCH_MIN = -89.0;
constexpr float FOV_MIN = 1.0;
constexpr float FOV_MAX = 45.0;

constexpr glm::vec3 DEFAULT_POSITION = {-5, 0.4, 5};
constexpr glm::vec3 DEFAULT_FRONT = {0, 0, -1};
constexpr glm::vec3 DEFAULT_UP = {0, 1, 0};
}  // namespace Camera

namespace Chunk {
constexpr uint LENGTH = 16;
constexpr uint HEIGHT = 256;
constexpr uint RENDER_DISTANCE_CHUNKS = 4;
constexpr uint RENDER_DISTANCE_BLOCKS = RENDER_DISTANCE_CHUNKS * LENGTH;
}  // namespace Chunk

}  // namespace Constants
