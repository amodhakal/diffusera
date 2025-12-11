#pragma once
#include <glm/glm.hpp>

namespace InigoNoise {
struct Noise2D {
  float value;        // noise value in [-1,1]
  glm::vec2 deriv;    // derivatives d/dx, d/dy (same units as input)
};

// 2D value noise with analytic derivatives (Inigo Quilez method)
Noise2D noised(const glm::vec2 &p);

// fbm using the above noised() with analytic derivative accumulation
Noise2D fbm(const glm::vec2 &p, int octaves = 6, float lacunarity = 2.0f,
            float gain = 0.5f);
}  // namespace InigoNoise
