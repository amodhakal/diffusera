#pragma once
#include <sys/types.h>

#include "shader.h"

class ChunkManager {
 public:
  ChunkManager();
  void load(const Shader &shader);
  void render(glm::vec3 cameraPosition);

 private:
  Shader m_Shader;
};
