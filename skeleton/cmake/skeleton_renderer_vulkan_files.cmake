set(SKELETON_RENDERER_VULKAN_FILES
  src/skeleton/renderer/vulkan/vulkan_check.hpp
  src/skeleton/renderer/vulkan/vulkan_check.cpp
  src/skeleton/renderer/vulkan/vulkan_device.cpp
  src/skeleton/renderer/vulkan/vulkan_device_queue_families.hpp
  src/skeleton/renderer/vulkan/vulkan_device_queue_families.cpp
  src/skeleton/renderer/vulkan/vulkan_instance.cpp
  src/skeleton/renderer/vulkan/vulkan_physical_device.cpp
  src/skeleton/renderer/vulkan/vulkan_renderer.hpp
  src/skeleton/renderer/vulkan/vulkan_renderer.cpp
)
source_group("src/skeleton/renderer/vulkan" FILES ${SKELETON_RENDERER_VULKAN_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_RENDERER_VULKAN_FILES}
)
