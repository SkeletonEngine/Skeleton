#pragma once

struct GLFWwindow;

namespace Skeleton {

class Window {
public:
  Window();
  ~Window();

public:
  bool IsOpen() const;
  void PollEvents();

private:
  GLFWwindow* window;
};

}
