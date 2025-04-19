// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <functional>
#include <string>
#include "skeleton/window/window_backend.hpp"

struct GLFWwindow;

namespace Skeleton {

class Window {
 public:
  virtual ~Window() { };

 public:
  virtual WindowBackend GetBackend() const = 0;

 public:
  // To be called by the engine once per frame
  virtual void PollEvents() const = 0;

 public:
  // To allow the renderer to respond to framebuffer resize events
  virtual void RegisterFramebufferSizeCallback(std::function<void(int width, int height)> callback) = 0;

 public:
  // Returns true if the window has not yet recieved a close event or platform equivalent
  virtual bool IsOpen() const = 0;

 public:
  virtual int GetFramebufferWidth() const = 0;
  virtual int GetFramebufferHeight() const = 0;

 public:
  virtual void SetTitle(const std::string& title) = 0;
};

}  // namespace Skeleton
