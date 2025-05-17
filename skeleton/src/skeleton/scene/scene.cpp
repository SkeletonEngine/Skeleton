// Copyright 2024-2025 SkeletonEngine

#include "skeleton/scene/scene.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <entt/entt.hpp>
#include <uuid.h>
#include "skeleton/scene/components/components.hpp"

namespace Skeleton {

entt::entity GetRootNode(entt::registry* scene) {
  entt::entity root = entt::null;

  // Create a view over the scene and grab the root entity
  // The root entity will be the only entity whose parent is entt::null
  scene->view<ParentComponent>().each([&](const auto entity, const auto& parent_component) {
    if (parent_component.parent == entt::null) {
      root = entity;
    }
  });

  return root;
}

entt::entity GetNodeByUuid(entt::registry* scene, const uuids::uuid& uuid) {
  entt::entity result = entt::null;

  scene->view<UuidComponent>().each([&](const auto entity, const auto& other_uuid) {
    if (uuid == other_uuid.uuid) {
      result = entity;
    }
  });

  return result;
}

}  // namespace Skeleton
