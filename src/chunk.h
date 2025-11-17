#pragma once



#include <sys/types.h>

#include <vector>

struct ChunkPosition {
  int xPosition;
  int zPosition;
};

class Chunk {
 public:
  Chunk(const ChunkPosition &position);
  void render();

 private:
  uint m_VAO;
  uint m_VBO;

  std::vector<float> m_VboData;
};
