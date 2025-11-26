#pragma once

#include <sys/types.h>

#include <vector>

#include "config.h"
#include "noise/noise.h"

enum BlockType { AIR, GRASS, DIRT };

enum BlockNormal {
  RIGHT_LEFT_NORMAL = 0,
  FRONT_BACK_NORMAL = 1,
  BOTTOM_NORMAL = 2,
  TOP_NORMAL = 3
};

typedef BlockType BlockStore[Constants::Chunk::LENGTH][Constants::Chunk::HEIGHT]
                            [Constants::Chunk::LENGTH];

class Chunk {
 public:
  Chunk() = default;

  std::vector<float> generateMeshData(const glm::vec2& position,
                                      const FastNoiseLite& noiseGenerator);
  void useMeshData(std::vector<float>& meshData);

  void render();
  void cleanup();

 private:
  uint m_VAO;
  uint m_VBO;
  uint m_VboSize;
};
