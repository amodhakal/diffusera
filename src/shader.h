#pragma once
#include <sys/types.h>

class Shader {
 public:
  Shader();
  void load(const char* vertexPath, const char* fragmentPath);
  uint getId();

 private:
  uint m_Id;
};
