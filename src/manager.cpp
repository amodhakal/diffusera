#include "manager.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

#include "chunk.h"

ChunkManager::ChunkManager(const Shader &shader)
    : m_Shader(shader), m_VboData({}) {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
}

/**
 * Recalculates all of the vbo data for rendering
 */
void ChunkManager::draw() {
  // Clear the previous vbo data
  // TODO: Optimize such that outside render distance chunks are removed and new
  // chunks are rendered
  // TODO: Do the chunk rendering in another thread, and sync the state changes
  // properly
  // TODO: Do not show the faces who are covered

  m_VboData.clear();
  Chunk chunk1(0, 0);
  auto chunkData = chunk1.getVboData();
  m_VboData.insert(m_VboData.end(), chunkData.begin(), chunkData.end());

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

/**
 * Displays the vbo data to the screen
 */
void ChunkManager::render() {
  glUseProgram(m_Shader.getId());
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_VboData.size() / 9);
}
