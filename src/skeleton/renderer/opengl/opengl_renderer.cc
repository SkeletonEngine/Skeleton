#include "opengl_renderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace Skeleton {
namespace Renderer {

static GLFWwindow* window;

void RenderScene() {
  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);
}

}
}

namespace Skeleton {
namespace Renderer {
namespace Internal {

void ApplyGlfwWindowHints() {
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
}

void CreateContext(GLFWwindow* glfw_window) {
  window = glfw_window;
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  
  glfwSwapInterval(1);
  glClearColor(0.2, 0.4, 0.6, 1.0);
}

}
}
}
