#pragma once
#include <sys/types.h>

#include <map>
#include <unordered_map>
#include <vector>

#include "chunk.h"
#include "shader.h"

class ChunkManager {
 public:
  ChunkManager();
  void load(const Shader &shader);
  void render(glm::vec3 position);

 private:
  Shader m_Shader;
  std::unordered_map<ChunkPosition, Chunk> m_Chunks;
  std::vector<float> m_VboData;

  void update(glm::vec3 position);

  uint m_VAO;
  uint m_VBO;

  bool m_IsLatest;
};
