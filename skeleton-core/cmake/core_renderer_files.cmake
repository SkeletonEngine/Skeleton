set(SKELETON_CORE_RENDERER_FILES
  include/skeleton/renderer/renderer_settings.hpp
  include/skeleton/renderer/renderer_backend.hpp
  src/skeleton/renderer/renderer.hpp
)
source_group("src/skeleton-core/renderer" FILES ${SKELETON_CORE_RENDERER_FILES})

set(SKELETON_CORE_FILES
  ${SKELETON_CORE_FILES}
  ${SKELETON_CORE_RENDERER_FILES}
)
