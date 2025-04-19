// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <functional>
#include <string>
#include "skeleton/application_settings.hpp"
#include "skeleton/window/window.hpp"

struct GLFWwindow;

namespace Skeleton {

class GlfwWindow : public Window {
 public:
  GlfwWindow(RendererBackend renderer_backend);
  virtual ~GlfwWindow();

 public:
  virtual WindowBackend GetBackend() const override { return WindowBackend::kGlfw; };

 public:
  // To be called by the engine once per frame
  virtual void PollEvents() const override;

 public:
  // To allow the renderer to respond to framebuffer resize events
  virtual void RegisterFramebufferSizeCallback(std::function<void(int width, int height)> callback) override;

 public:
  // Should return the underlying GLFWwindow, HWND, etc.
  GLFWwindow* GetNativeWindowHandle() const;

 public:
  // Returns true if the window has not yet recieved a close event or platform equivalent
  bool IsOpen() const override;

 public:
  virtual int GetFramebufferWidth() const override;
  virtual int GetFramebufferHeight() const override;

 public:
  virtual void SetTitle(const std::string& title) override;

 private:
  GLFWwindow* glfw_window_;
  std::function<void(int width, int height)> framebuffer_size_callback_;
};

}  // namespace Skeleton
