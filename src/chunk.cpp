#include "chunk.h"

#include "config.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <OpenGL/gl.h>

Chunk::Chunk(const ChunkPosition &position) {
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
