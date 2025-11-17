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

void ChunkManager::load(const Shader& shader) {
  m_Shader = shader;
}

void ChunkManager::render(glm::vec3 cameraPosition) {
  for (auto it = m_Chunks.begin(); it != m_Chunks.end();) {
    const ChunkPosition& position = it->first;
    float xDisplacement =
        (position.xPosition * Constants::Chunk::LENGTH) - cameraPosition.x;
    float zDisplacement =
        (position.zPosition * Constants::Chunk::LENGTH) - cameraPosition.z;

    float distanceSquared =
        (xDisplacement * xDisplacement) + (zDisplacement * zDisplacement);
    if (distanceSquared > Constants::Chunk::RENDER_DISTANCE_BLOCKS *
                              Constants::Chunk::RENDER_DISTANCE_BLOCKS) {
      it = m_Chunks.erase(it);
      continue;
    }

    ++it;
  }

  int currentChunkX =
      static_cast<int>(std::floor(cameraPosition.x / Constants::Chunk::LENGTH));
  int currentChunkZ =
      static_cast<int>(std::floor(cameraPosition.z / Constants::Chunk::LENGTH));

  for (int chunkX = currentChunkX - Constants::Chunk::RENDER_DISTANCE_CHUNKS;
       chunkX <= currentChunkX + Constants::Chunk::RENDER_DISTANCE_CHUNKS;
       chunkX++) {
    for (int chunkZ = currentChunkZ - Constants::Chunk::RENDER_DISTANCE_CHUNKS;
         chunkZ <= currentChunkZ + Constants::Chunk::RENDER_DISTANCE_CHUNKS;
         chunkZ++) {
      ChunkPosition position = {chunkX, chunkZ};
      if (m_Chunks.find(position) == m_Chunks.end()) {
        m_Chunks.emplace(position, Chunk(position));
      }
    }
  }

  glUseProgram(m_Shader.getId());
  for (auto& value : m_Chunks) {
    Chunk& chunk = value.second;
    chunk.render();
  }
}
