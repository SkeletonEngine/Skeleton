// Copyright 2024-2025 SkeletonEngine

#include "skeleton/scene/entity.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <entt/entt.hpp>
#include "skeleton/scene/components/components.hpp"

namespace Skeleton {

entt::registry LoadScene(const std::string& path) {
  entt::registry scene;
  entt::entity root = CreateEmptyEntity(&scene, entt::null, "3D Scene");
  scene.emplace<ClearColorComponent>(root, Color(0x91b1b5ff));

  // TODO(jack): Add actual functionality to the camera node
  entt::entity camera = CreateEmptyEntity(&scene, root, "Camera");

  // TODO(jack): Add actual functionality to the model node
  entt::entity square = CreateEmptyEntity(&scene, root, "Square Model");

  return scene;
}

void SaveScene(const std::string& path, entt::registry* scene) {

}

}  // namespace Skeleton
