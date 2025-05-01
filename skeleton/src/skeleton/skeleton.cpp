// Copyright 2024-2025 SkeletonEngine

#include "skeleton/skeleton.hpp"
#include "skeleton/core/core.hpp"

#include <entt/entt.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/entity.hpp"

namespace Skeleton {

void StartApplication(Window* window, Renderer* renderer) {
  entt::registry scene;
  entt::entity root = CreateEmptyEntity(&scene, entt::null, "Root");

  // TODO(jack): Remove test code
  entt::entity node1 = CreateEmptyEntity(&scene, root, "Node 1");
  entt::entity node2 = CreateEmptyEntity(&scene, root, "Node 2");
  entt::entity node3 = CreateEmptyEntity(&scene, root, "Node 3");

  entt::entity node4 = CreateEmptyEntity(&scene, node2, "Node 4");
  entt::entity node5 = CreateEmptyEntity(&scene, node2, "Node 5");

  entt::entity node6 = CreateEmptyEntity(&scene, node3, "Node 6");

  entt::entity node7 = CreateEmptyEntity(&scene, node6, "Node 7");

  renderer->SetScene(&scene, root);

  while (window->IsOpen()) {
    window->PollEvents();
    renderer->RenderFrame();
  }
}

}  // namespace Skeleton
