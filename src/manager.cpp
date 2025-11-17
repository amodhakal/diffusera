#include "manager.h"

#include <cmath>

#include "chunk.h"
#include "config.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

ChunkManager::ChunkManager() {
}

void ChunkManager::load(const Shader &shader) {
  m_Shader = shader;
}

void ChunkManager::render(glm::vec3 cameraPosition) {
  // TODO: Handle chunk borders when camera position is -
  int currentChunkX = floor(cameraPosition.x);
  int currentChunkZ = floor(cameraPosition.z);

  currentChunkX /= Constants::Chunk::LENGTH;
  currentChunkZ /= Constants::Chunk::LENGTH;

  glUseProgram(m_Shader.getId());

  Chunk chunk({currentChunkX, currentChunkZ});
  chunk.render();
}
