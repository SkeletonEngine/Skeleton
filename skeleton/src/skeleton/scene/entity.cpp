// Copyright 2024-2025 SkeletonEngine

#include "skeleton/scene/entity.hpp"
#include "skeleton/core/core.hpp"

#include <string>
#include <entt/entt.hpp>
#include "skeleton/scene/components/components.hpp"

namespace Skeleton {

entt::entity CreateEmptyEntity(entt::registry* scene, entt::entity parent, const std::string& name) {
  entt::entity e = scene->create();

  // Add common components
  scene->emplace<UuidComponent>(e, uuids::uuid_system_generator { }());
  scene->emplace<NameComponent>(e, name);
  scene->emplace<ChildrenComponent>(e);
  scene->emplace<ParentComponent>(e, parent);
  // In the future we'll likely also have this:
  // scene->emplace<TransformComponent>(e);

  // Grab parent component and add this entity to its list of children
  if (parent != entt::null) {
    auto& children = scene->get<ChildrenComponent>(parent).children;
    children.push_back(e);
  }

  return e;
}

}  // namespace Skeleton
