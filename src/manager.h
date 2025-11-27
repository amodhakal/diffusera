#pragma once
#include <noise/noise.h>
#include <sys/types.h>

#include <future>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "camera.h"
#include "chunk.h"
#include "shader.h"

namespace std {
template <>
struct hash<glm::vec2> {
  size_t operator()(glm::vec2 const& v) const noexcept {
    size_t h1 = std::hash<float>{}(v.s);
    size_t h2 = std::hash<float>{}(v.t);
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
  }
};

}  // namespace std

class ChunkManager {
 public:
  ChunkManager();
  void load();
  void render(const Camera* camera, Shader& shader);

 private:
  FastNoiseLite m_NoiseGenerator;
  std::unordered_map<glm::vec2, Chunk> m_ProcessedChunks;
  std::unordered_set<glm::vec2> m_ProcessingPositions;

  std::unordered_map<glm::vec2, std::future<std::vector<float>>>
      m_ProcessingChunks;
};
