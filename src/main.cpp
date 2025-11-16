#include <cstdlib>

#include "application.h"

int main() {
  Application linterra("Linterra");
  while (linterra.isRunning()) {
    linterra.update();
  }

  return EXIT_SUCCESS;
}
