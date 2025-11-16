#pragma once
#include <vector>
#include <sys/types.h>
#include "shader.h"

class ChunkManager {
 public:
  ChunkManager(const Shader &shader);
  void draw();
  void render();

 private:
  Shader m_Shader;
  std::vector<float> m_VboData;
  uint m_VAO;
  uint m_VBO;
};
