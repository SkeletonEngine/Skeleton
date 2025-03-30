// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <functional>
#include "skeleton/application_settings.hpp"

struct GLFWwindow;

namespace Skeleton {

class Window {
 public:
  explicit Window(const ApplicationSettings& settings);
  ~Window();

 public:
  /* Should be called once per frame */
  void PollEvents() const;

  /* Returns true if the window has not yet recieved a close event or platform equivalent */
  bool IsOpen() const;

  /* Allows renderer to handle framebuffer resize events */
  void RegisterFramebufferSizeCallback(std::function<void(int width, int height)> callback);

 public:
  GLFWwindow* GetGlfwWindowHandle() const;
  int GetFramebufferWidth() const;
  int GetFramebufferHeight() const;

 private:
  GLFWwindow* glfw_window_;
  std::function<void(int width, int height)> framebuffer_size_callback_;
};

}  // namespace Skeleton
