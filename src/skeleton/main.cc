#include "window.h"
#include "renderer/renderer.h"

int main() {
  Skeleton::Window window;

  while (window.IsOpen()) {
    window.PollEvents();
    
    Skeleton::Renderer::RenderScene();
  }
}
