#include "window.h"
#include "renderer/renderer.h"

int main() {
  Skeleton::Renderer::Init(Skeleton::Renderer::Backend::kOpenGl);
  Skeleton::Window window;

  while (window.IsOpen()) {
    window.PollEvents();
  }
}
