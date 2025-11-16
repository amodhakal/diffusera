#pragma once
#include <sys/types.h>

#include <vector>

#include "shader.h"

class ChunkManager {
 public:
  ChunkManager();
  void load(const Shader &shader);
  void draw();
  void render();

 private:
  Shader m_Shader;
  std::vector<float> m_VboData;
  uint m_VAO;
  uint m_VBO;
};
