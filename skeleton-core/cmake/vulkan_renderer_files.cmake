set(SKELETON_CORE_RENDERER_VULKAN_FILES
  src/skeleton/renderer/vulkan/vulkan_check.hpp
  src/skeleton/renderer/vulkan/vulkan_check.cpp
  src/skeleton/renderer/vulkan/vulkan_renderer.hpp
  src/skeleton/renderer/vulkan/vulkan_renderer.cpp
)
source_group("src/skeleton-core/renderer/vulkan" FILES ${SKELETON_CORE_RENDERER_VULKAN_FILES})

set(SKELETON_CORE_FILES
  ${SKELETON_CORE_FILES}
  ${SKELETON_CORE_RENDERER_VULKAN_FILES}
)
