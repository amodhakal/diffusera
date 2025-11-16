#pragma once
#include <sys/types.h>

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Shader {
 public:
  Shader();
  void load(const char* vertexPath, const char* fragmentPath);
  uint getId();

  void newUniform(const char* name);
  void setUniformMat4(const char* name, float* values);

 private:
  uint m_Id;
  std::unordered_map<std::string, int> m_Uniforms;
};
