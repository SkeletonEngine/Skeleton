set(SKELETON_RENDERER_FILES
  include/skeleton/renderer/renderer_settings.hpp
  include/skeleton/renderer/renderer_backend.hpp
  src/skeleton/renderer/renderer.hpp
)
source_group("src/skeleton/renderer" FILES ${SKELETON_RENDERER_FILES})

set(SKELETON_RENDERER_COMMON_FILES
  src/skeleton/renderer/common/spv_file.hpp
  src/skeleton/renderer/common/spv_file.cpp
)
source_group("src/skeleton/renderer/common" FILES ${SKELETON_RENDERER_COMMON_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_RENDERER_FILES}
  ${SKELETON_RENDERER_COMMON_FILES}
)
