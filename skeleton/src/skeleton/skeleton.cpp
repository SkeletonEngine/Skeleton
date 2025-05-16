// Copyright 2024-2025 SkeletonEngine

#include "skeleton/skeleton.hpp"
#include "skeleton/core/core.hpp"

#include <entt/entt.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/scene_serializer.hpp"

namespace Skeleton {

void StartApplication(Window* window, Renderer* renderer) {
  entt::registry scene = LoadScene("skeleton/res/scenes/3d-scene-template.skscene");
  renderer->SetScene(&scene);

  while (window->IsOpen()) {
    window->PollEvents();
    renderer->RenderFrame();
  }
}

}  // namespace Skeleton
