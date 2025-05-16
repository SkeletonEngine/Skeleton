// Copyright 2024-2025 SkeletonEngine

#include "skeleton/scene/scene_serializer.hpp"
#include "skeleton/core/core.hpp"

#include <fstream>
#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "skeleton/scene/components/components.hpp"
#include "skeleton/scene/entity.hpp"
#include "skeleton/scene/scene.hpp"

namespace Skeleton {

struct CurrentCameraUuidComponent {
  uuids::uuid uuid;
};

static void DeserializeNode(entt::registry& scene, const nlohmann::json& json, entt::entity parent) {
  std::string name = json["name"];
  entt::entity entity = CreateEmptyEntity(&scene, parent, name);

  std::string uuid_string = json["uuid"];
  uuids::uuid uuid = uuids::uuid::from_string(uuid_string).value();
  scene.remove<UuidComponent>(entity);
  scene.emplace<UuidComponent>(entity, uuid);

  if (json.contains("camera")) {
    auto& camera = json["camera"];
    CameraComponent camera_component;
    camera_component.fov = camera["fov"];
    camera_component.clip_near = camera["clip_near"];
    camera_component.clip_far = camera["clip_far"];
    scene.emplace<CameraComponent>(entity, camera_component);
  }

  if (json.contains("current_camera")) {
    std::string current_camera_uuid_string = json["current_camera"];
    uuids::uuid current_camera_uuid = uuids::uuid::from_string(current_camera_uuid_string).value();
    scene.emplace<CurrentCameraUuidComponent>(entity, current_camera_uuid);
  }

  if (json.contains("clear_color")) {
    float r = json["clear_color"]["r"];
    float g = json["clear_color"]["g"];
    float b = json["clear_color"]["b"];
    float a = json["clear_color"]["a"];
    scene.emplace<ClearColorComponent>(entity, Color(r, g, b, a));
  }

  if (json.contains("children")) {
    for (const auto& child : json["children"]) {
      DeserializeNode(scene, child, entity);
    }
  }
}

void DeserializeLinks(entt::registry& scene) {
  // For each entity that has a current camera component, look up the camera entity by its uuid and set this entity to point to it
  scene.view<CurrentCameraUuidComponent>().each([&](const auto entity, const auto& camera_uuid) {
    printf("Looking for camera with uuid %s\n", uuids::to_string(camera_uuid.uuid).c_str());
    scene.view<UuidComponent>().each([&](const auto camera_entity, const auto& uuid) {
      if (uuid.uuid == camera_uuid.uuid) {
        scene.emplace<CurrentCameraComponent>(entity, camera_entity);
        scene.remove<CurrentCameraUuidComponent>(entity);
      }
    });
  });
}

entt::registry LoadScene(const std::string& path) {
  // Load file into a json object for parsing
  std::ifstream file(path);
  SK_ASSERT(file.is_open());
  nlohmann::json json;
  file >> json;
  file.close();

  // Recursively deserialize the scene
  entt::registry scene;
  DeserializeNode(scene, json, entt::null);
  DeserializeLinks(scene);
  return scene;
}

void SerializeNode(entt::registry& scene, nlohmann::json& json, entt::entity entity) {
  auto& name = scene.get<NameComponent>(entity).name;
  json["name"] = name;

  auto& uuid = scene.get<UuidComponent>(entity).uuid;
  json["uuid"] = uuids::to_string(uuid);

  if (scene.any_of<CameraComponent>(entity)) {
    auto& camera = scene.get<CameraComponent>(entity);
    json["camera"]["fov"] = camera.fov;
    json["camera"]["clip_near"] = camera.clip_near;
    json["camera"]["clip_far"] = camera.clip_far;
  }

  if (scene.any_of<ClearColorComponent>(entity)) {
    auto& clear_color = scene.get<ClearColorComponent>(entity).color;
    json["clear_color"]["r"] = clear_color.r;
    json["clear_color"]["g"] = clear_color.g;
    json["clear_color"]["b"] = clear_color.b;
    json["clear_color"]["a"] = clear_color.a;
  }

  if (scene.any_of<ChildrenComponent>(entity)) {
    auto& children = scene.get<ChildrenComponent>(entity).children;
    for (const auto& child : children) {
      nlohmann::json child_json;
      SerializeNode(scene, child_json, child);
      json["children"].push_back(child_json);
    }
  }
}

void SaveScene(const std::string& path, entt::registry* scene) {
  nlohmann::json json;
  entt::entity root = GetRootNode(scene);
  SerializeNode(*scene, json, root);

  std::ofstream file(path);
  file << json.dump(2);
  file.close();
}

}  // namespace Skeleton
