#include "chunk.h"

#include <glad/glad.h>

#include "config.h"

Chunk::Chunk(const ChunkPosition &position) {
  float grassHeight = 2;
  BlockType blocks[Constants::Chunk::LENGTH][Constants::Chunk::HEIGHT]
                  [Constants::Chunk::LENGTH];

  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockY = 0; blockY < grassHeight; blockY++) {
      for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
        blocks[blockX][blockY][blockZ] = BlockType::GRASS;
      }
    }
  }

  for (uint blockX = 0; blockX < Constants::Chunk::LENGTH; blockX++) {
    for (uint blockY = grassHeight; blockY < Constants::Chunk::HEIGHT;
         blockY++) {
      for (uint blockZ = 0; blockZ < Constants::Chunk::LENGTH; blockZ++) {
        blocks[blockX][blockY][blockZ] = BlockType::AIR;
      }
    }
  }

  // TODO Convert the blocks representation into a mesh, then the vbo data
  // TODO Remove the faces hidden inside other blocks

  float baseX = position.xPosition * Constants::Chunk::LENGTH;
  float baseZ = position.zPosition * Constants::Chunk::LENGTH;
  m_VboData = {
      // First triangle
      // Bottom-left
      baseX, 0.0f, baseZ, 0.0f, 0.0f, 1.0f, 0.0f, 0.6f, 0.1f,
      // Bottom-right
      baseX + Constants::Chunk::LENGTH, 0.0f, baseZ, 0.0f, 0.0f, 1.0f, 0.0f,
      0.6f, 0.1f,
      // Top-left
      baseX, 0.0f, baseZ + Constants::Chunk::LENGTH, 0.0f, 0.0f, 1.0f, 0.0f,
      0.6f, 0.1f,

      // Second triangle
      // Bottom-right
      baseX + Constants::Chunk::LENGTH, 0.0f, baseZ, 0.0f, 0.0f, 1.0f, 0.0f,
      0.6f, 0.1f,
      // Top-right
      baseX + Constants::Chunk::LENGTH, 0.0f, baseZ + Constants::Chunk::LENGTH,
      0.0f, 0.0f, 1.0f, 0.0f, 0.6f, 0.1f,
      // Top-left
      baseX, 0.0f, baseZ + Constants::Chunk::LENGTH, 0.0f, 0.0f, 1.0f, 0.0f,
      0.6f, 0.1f};

  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_VboData.size() * sizeof(float),
               m_VboData.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Chunk::render() {
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_VboData.size() / 9);
}
