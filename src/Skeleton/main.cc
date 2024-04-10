#include "input.hh"
#include "window.hh"
#include <fractal/fractal.hh>

int main() {
  using namespace Skeleton;
  
  Window::Create();
  Fractal::CreateContext();
  Fractal::SetWindow(Window::Get());
  
  while (Window::IsOpen()) {
    Input::Poll();
  }

  Fractal::DestroyContext();
  Window::Destroy();
}
