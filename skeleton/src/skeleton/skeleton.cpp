// Copyright 2024-2025 SkeletonEngine

#include "skeleton/skeleton.hpp"
#include "skeleton/core/core.hpp"

#include <entt/entt.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/entity.hpp"

namespace Skeleton {

void StartApplication(Window* window, Renderer* renderer) {
  entt::registry scene;
  entt::entity root = CreateEmptyEntity(&scene, entt::null, "3D Scene");
  scene.emplace<ClearColorComponent>(root, Color(0x91b1b5ff));

  // TODO(jack): Add actual functionality to the camera node
  entt::entity camera = CreateEmptyEntity(&scene, root, "Camera");

  // TODO(jack): Add actual functionality to the model node
  entt::entity square = CreateEmptyEntity(&scene, root, "Square Model");

  renderer->SetScene(&scene, root);

  while (window->IsOpen()) {
    window->PollEvents();
    renderer->RenderFrame();
  }
}

}  // namespace Skeleton
