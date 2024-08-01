#include "entity.h"

#include "components.h"

namespace Skeleton {

Entity::Entity(entt::registry& registry) : registry(registry) {
  entity = registry.create();
  AddComponent<NameComponent>("Entity");
}

}
