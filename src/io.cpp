#include "io.h"
#include <fstream>

const std::string IO::getFullFileContents(const char *filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    throw std::runtime_error("Couldn't open file: " + std::string(filePath));
  }

  std::string content, line;
  while (getline(file, line)) {
    content += line + "\n";
  }

  return content;
}
