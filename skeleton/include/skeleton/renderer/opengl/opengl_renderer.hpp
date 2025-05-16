// Copyright 2024-2025 SkeletonEngine

#pragma once
#include "skeleton/core/core.hpp"

#include <string>
#include <entt/entt.hpp>
#include "skeleton/renderer/renderer.hpp"
#include "skeleton/renderer/renderer_settings.hpp"
#include "skeleton/window/window.hpp"

struct GLFWwindow;

namespace Skeleton::OpenGl {

class OpenGlRenderer : public Renderer {
 public:
  explicit OpenGlRenderer(const RendererSettings& settings);

 public:
  virtual RendererBackend GetBackend() const override { return RendererBackend::kOpenGl; }

 public:
  virtual void RenderFrame() override;

 public:
  virtual std::string GetRendererString() const override;

 public:
  virtual void SetScene(entt::registry* scene) override;

 private:
  GLFWwindow* glfw_window_;
};

}  // namespace Skeleton::OpenGl
