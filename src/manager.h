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
  void load(const Shader& shader);
  void render(const Camera& cmaera);

 private:
  Shader m_Shader;
  FastNoiseLite m_NoiseGenerator;

  std::unordered_map<ChunkPosition, Chunk> m_Chunks;
};
