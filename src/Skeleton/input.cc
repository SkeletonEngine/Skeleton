#include "input.hh"

#include <GLFW/glfw3.h>

namespace Skeleton {
namespace Input {

void Poll() {
  glfwPollEvents();
}

}
}
