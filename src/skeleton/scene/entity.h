#pragma once

#include <entt/entity/registry.hpp>

#include "components.h"

namespace Skeleton {

class Entity {
public:
  Entity(entt::registry& registry);

public:
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			// assert(!HasComponent<T>(), "Entity already has component!");
			T& component = registry.emplace<T>(entity, std::forward<Args>(args)...);
			// scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args) {
			T& component = registry.emplace_or_replace<T>(entity, std::forward<Args>(args)...);
			// scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent() {
			// assert(HasComponent<T>(), "Entity does not have component!");
			return registry.get<T>(entity);
		}

		template<typename T>
		bool HasComponent() {
			return registry.all_of<T>(entity);
		}

		template<typename T>
		void RemoveComponent() {
			// assert(HasComponent<T>(), "Entity does not have component!");
			registry.remove<T>(entity);
		}

private:
  entt::registry& registry;
	entt::entity entity;
};

}
