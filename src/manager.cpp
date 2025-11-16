#include "manager.h"

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>

ChunkManager::ChunkManager() : m_VboData({}), m_IsLatest(false) {
}

void ChunkManager::load(const Shader &shader) {
  m_Shader = shader;
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
}

/**
 * Displays the vbo data to the screen
 */
void ChunkManager::render() {
  update();

  glUseProgram(m_Shader.getId());
  glBindVertexArray(m_VAO);
  glDrawArrays(GL_TRIANGLES, 0, m_VboData.size() / 9);
}

void ChunkManager::update() {
  if (m_IsLatest) {
    return;
  }

  // Clear the previous vbo data
  // TODO: Optimize such that outside render distance chunks are removed and new
  // chunks are rendered
  // TODO: Do the chunk rendering in another thread, and sync the state changes
  // properly
  // TODO: Do not show the faces who are covered

  m_VboData.clear();
  // Chunk chunk1(0, 0);
  // auto chunkData = chunk1.getVboData();
  // m_VboData.insert(m_VboData.end(), chunkData.begin(), chunkData.end());

  // TODO: Setup properly
  m_VboData = {
      // Vertex 1
      -0.5f, 0.0f, -0.5f,  // position
      0.0f, 1.0f, 0.0f,    // normal
      0.0f, 0.0f, 1.0f,    // color (blue)
      // Vertex 2
      0.0f, 0.0f, 0.5f,  // position
      0.0f, 1.0f, 0.0f,  // normal
      0.0f, 0.0f, 1.0f,  // color (blue)
      // Vertex 3
      0.5f, 0.0f, -0.5f,  // position
      0.0f, 1.0f, 0.0f,   // normal
      0.0f, 0.0f, 1.0f    // color (blue)
  };

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

  m_IsLatest = true;
}
