#include "texture.h"

#include <glad/glad.h>

#include <stdexcept>
#include <string>

void Texture::loadFromFile(const char* path) {
  int width, height, channels;
  stbi_uc* data = stbi_load(path, &width, &height, &channels, 4);
  if (!data) {
    throw std::runtime_error(std::string("Failed to load texture: ") + path);
  }

  glGenTextures(1, &m_Id);
  glBindTexture(GL_TEXTURE_2D, m_Id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

void Texture::bindToUnit(int unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, m_Id);
}

Texture::~Texture() {
  if (m_Id != 0) {
    glDeleteTextures(1, &m_Id);
  }
}
