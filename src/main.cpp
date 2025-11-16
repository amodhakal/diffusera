#include <cstdlib>

#include "application.h"

int main() {
  Application linterra("Linterra");
  while (linterra.isRunning()) {
    linterra.run();
  }

  return EXIT_SUCCESS;
}
