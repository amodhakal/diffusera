#pragma once
#include <sys/types.h>

#include <unordered_map>

#include "chunk.h"
#include "shader.h"
#include <noise/noise.h>

class ChunkManager {
 public:
  ChunkManager();
  void load(const Shader &shader);
  void render(glm::vec3 cameraPosition);

 private:
  Shader m_Shader;
  FastNoiseLite m_NoiseGenerator;

  std::unordered_map<ChunkPosition, Chunk> m_Chunks;
};
