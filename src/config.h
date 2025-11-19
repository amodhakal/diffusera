#pragma once
#include <sys/types.h>

#include <glm/glm.hpp>

#include "noise/noise.h"

namespace Constants {
constexpr auto VERTEX_PATH = "./shaders/shaders.vert";
constexpr auto FRAGMENT_PATH = "./shaders/shaders.frag";

constexpr uint SCR_WIDTH = 800;
constexpr uint SCR_HEIGHT = 600;

constexpr auto BG_COLOR = glm::vec4(0.1, 0.1, 0.1, 1.0);

constexpr bool DO_TRIANGLE_LINE = false;

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

constexpr glm::vec3 DEFAULT_POSITION = {5, 90, 5};
constexpr glm::vec3 DEFAULT_FRONT = {0, 0, -1};
constexpr glm::vec3 DEFAULT_UP = {0, 1, 0};
}  // namespace Camera

namespace Chunk {
constexpr int LENGTH = 16;
constexpr int HEIGHT = 256;
constexpr int RENDER_DISTANCE_CHUNKS = 8;
constexpr int RENDER_DISTANCE_BLOCKS = RENDER_DISTANCE_CHUNKS * LENGTH;
constexpr int MAX_BLOCK_HEIGHT = HEIGHT / 2.0;
}  // namespace Chunk

namespace Noise {
constexpr auto NOISE_TYPE = FastNoiseLite::NoiseType_Perlin;
constexpr auto FRACTAL_TYPE = FastNoiseLite::FractalType_FBm;
constexpr float FRACTAL_OCTAVE = 5.0;
constexpr float FRACTAL_GAIN = 0.5;
constexpr float FRACTAL_LACUNARITY = 1.0;
constexpr float FREQUENCY = 0.01;

}  // namespace Noise

}  // namespace Constants
