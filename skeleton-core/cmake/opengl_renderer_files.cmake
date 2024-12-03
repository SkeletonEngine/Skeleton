set(SKELETON_CORE_RENDERER_OPENGL_FILES
  src/skeleton/renderer/opengl/opengl_renderer.hpp
  src/skeleton/renderer/opengl/opengl_renderer.cpp
)
source_group("src/skeleton-core/renderer/opengl" FILES ${SKELETON_CORE_RENDERER_OPENGL_FILES})

set(SKELETON_CORE_FILES
  ${SKELETON_CORE_FILES}
  ${SKELETON_CORE_RENDERER_OPENGL_FILES}
)
