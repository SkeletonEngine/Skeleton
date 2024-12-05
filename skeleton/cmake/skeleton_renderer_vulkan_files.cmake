set(SKELETON_RENDERER_VULKAN_FILES
  src/skeleton/renderer/vulkan/vulkan_check.hpp
  src/skeleton/renderer/vulkan/vulkan_check.cpp
  src/skeleton/renderer/vulkan/vulkan_renderer.hpp
  src/skeleton/renderer/vulkan/vulkan_renderer.cpp
)
source_group("src/skeleton/renderer/vulkan" FILES ${SKELETON_RENDERER_VULKAN_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_RENDERER_VULKAN_FILES}
)
