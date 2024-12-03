#pragma once

#include "skeleton/application_settings.hpp"

struct GLFWwindow;

namespace Skeleton {

class Window {
public:
  Window(const ApplicationSettings& settings);
  ~Window();
  
public:
  void PollEvents() const;
  
public:
  bool IsOpen() const;

private:
  GLFWwindow* glfw_window;
};

}
