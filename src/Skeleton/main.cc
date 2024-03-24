#include "input.hh"
#include "window.hh"

int main() {
  using namespace Skeleton;
  
  Window::Create();
  
  while (Window::IsOpen()) {
    Input::Poll();
  }
  
  Window::Destroy();
}
