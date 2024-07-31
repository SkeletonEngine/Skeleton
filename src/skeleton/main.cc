#include "window.h"

int main() {
  Skeleton::Window window;

  while (window.IsOpen()) {
    window.PollEvents();
  }
}
