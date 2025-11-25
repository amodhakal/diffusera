#pragma once
#include <noise/noise.h>
#include <sys/types.h>

#include <unordered_map>

#include "camera.h"
#include "chunk.h"
#include "shader.h"

class ChunkManager {
 public:
  ChunkManager();
  void load();
  void render(const Camera& camera, Shader& shader);

 private:
  FastNoiseLite m_NoiseGenerator;
  std::unordered_map<ChunkPosition, Chunk> m_Chunks;
};
