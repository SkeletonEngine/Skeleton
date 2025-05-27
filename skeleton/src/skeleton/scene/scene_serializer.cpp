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

  if (json.contains("uuid")) {
    std::string uuid_string = json["uuid"];
    uuids::uuid uuid = uuids::uuid::from_string(uuid_string).value();
    scene.remove<UuidComponent>(entity);
    scene.emplace<UuidComponent>(entity, uuid);
  }

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

  bool has_transform = false;

  if (json.contains("translation")) {
    auto& translation = json["translation"];
    TranslationComponent translation_component;
    translation_component.translation.x = translation[0];
    translation_component.translation.y = translation[1];
    translation_component.translation.z = translation[2];
    scene.emplace<TranslationComponent>(entity, translation_component);
    has_transform = true;
  }

  if (json.contains("rotation")) {
    auto& rotation = json["rotation"];
    RotationComponent rotation_component;
    rotation_component.rotation.x = rotation[0];
    rotation_component.rotation.y = rotation[1];
    rotation_component.rotation.z = rotation[2];
    scene.emplace<RotationComponent>(entity, rotation_component);
    has_transform = true;
  }

  if (has_transform) {
    TransformComponent transform_component;
    transform_component.transform = glm::mat4(1.0f);  // Initialize to identity matrix
    transform_component.matrix_dirty = true;  // Mark as dirty to recalculate
    scene.emplace<TransformComponent>(entity, transform_component);
  }

  if (json.contains("children")) {
    for (const auto& child : json["children"]) {
      DeserializeNode(scene, child, entity);
    }
  }
}

static void DeserializeLinks(entt::registry& scene) {
  scene.view<CurrentCameraUuidComponent>().each([&](const auto entity, const auto& uuid) {
    entt::entity camera = GetNodeByUuid(&scene, uuid.uuid);
    scene.emplace<CurrentCameraComponent>(entity, camera);
    scene.remove<CurrentCameraUuidComponent>(entity);
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

  if (scene.any_of<CurrentCameraComponent>(entity)) {
    entt::entity camera = scene.get<CurrentCameraComponent>(entity).current_camera;
    auto& uuid = scene.get<UuidComponent>(camera).uuid;
    json["current_camera"] = uuids::to_string(uuid);
  }

  if (scene.any_of<TranslationComponent>(entity)) {
    auto& translation = scene.get<TranslationComponent>(entity).translation;
    json["translation"][0] = translation.x;
    json["translation"][1] = translation.y;
    json["translation"][2] = translation.z;
  }

  if (scene.any_of<RotationComponent>(entity)) {
    auto& rotation = scene.get<RotationComponent>(entity).rotation;
    json["rotation"][0] = rotation.x;
    json["rotation"][1] = rotation.y;
    json["rotation"][2] = rotation.z;
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
