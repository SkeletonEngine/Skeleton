set(SKELETON_RENDERER_FILES
  include/skeleton/renderer/renderer_settings.hpp
  include/skeleton/renderer/renderer_backend.hpp
  src/skeleton/renderer/renderer.hpp
)
source_group("src/skeleton/renderer" FILES ${SKELETON_RENDERER_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_RENDERER_FILES}
)
