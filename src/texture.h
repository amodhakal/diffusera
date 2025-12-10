#pragma once
#include <sys/types.h>
#include <stb/image.h>


class Texture {
 public:
  Texture() = default;
  ~Texture();

  // Load from disk and create GL texture. Throws on failure.
  void loadFromFile(const char* path);

  // Bind to texture unit
  void bindToUnit(int unit) const;

  unsigned int getId() const { return m_Id; }

 private:
  unsigned int m_Id = 0;
};
