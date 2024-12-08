#pragma once
#include "skeleton/core/core.hpp"

#include "skeleton/application_settings.hpp"

struct GLFWwindow;

namespace Skeleton {

class Window {
public:
  Window(const ApplicationSettings& settings);
  ~Window();
  
public:
  void PollEvents() const;
  bool IsOpen() const;

public:
  GLFWwindow* GetGlfwWindowHandle() const;

private:
  GLFWwindow* glfw_window;
};

}
