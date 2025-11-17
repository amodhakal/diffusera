#include "manager.h"

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
  // Delete and add new chunks as needed

  glUseProgram(m_Shader.getId());

  // Go through each chunk and render it
}

// Each time
// glBindVertexArray(m_VAO);
// glDrawArrays(GL_TRIANGLES, 0, m_VboData.size() / 9);

// Constructor
// glGenVertexArrays(1, &m_VAO);
// glGenBuffers(1, &m_VBO);

// glBindVertexArray(m_VAO);
// glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
// glBufferData(GL_ARRAY_BUFFER, m_VboData.size() * sizeof(float),
//              m_VboData.data(), GL_STATIC_DRAW);

// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void
// *)0); glEnableVertexAttribArray(0); glVertexAttribPointer(1, 3, GL_FLOAT,
// GL_FALSE, 9 * sizeof(float),
//                       (void *)(3 * sizeof(float)));
// glEnableVertexAttribArray(1);

// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float),
//                       (void *)(6 * sizeof(float)));
// glEnableVertexAttribArray(2);

// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
