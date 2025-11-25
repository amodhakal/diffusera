#include "shader.h"

#include "io.h"

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include <string>

uint setupShaders(const char* filePath, const uint shaderType) {
  const std::string shaderCodeRaw = IO::getFullFileContents(filePath);
  const char* shaderCode = shaderCodeRaw.c_str();

  uint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderCode, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    throw std::runtime_error("Shader (" + std::string(filePath) +
                             ") compilation: " + std::string(infoLog));
  }

  return shader;
}

Shader::Shader() : m_Uniforms({}) {
}

void Shader::load(const char* vertexPath, const char* fragmentPath) {
  uint vertexShader = setupShaders(vertexPath, GL_VERTEX_SHADER);
  uint fragmentShader = setupShaders(fragmentPath, GL_FRAGMENT_SHADER);

  int success;
  char infoLog[512];

  uint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    throw std::runtime_error("Shader linking: " + std::string(infoLog));
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  m_Id = shaderProgram;
}

void Shader::use() {
  glUseProgram(m_Id);
}

uint Shader::getId() {
  return m_Id;
}

void Shader::newUniform(const char* name) {
  m_Uniforms[name] = glGetUniformLocation(m_Id, name);
}

void Shader::setUniformMat4(const char* name, float* values) {
  glUniformMatrix4fv(m_Uniforms[name], 1, GL_FALSE, values);
}
