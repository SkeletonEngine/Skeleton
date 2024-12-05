set(SKELETON_RENDERER_OPENGL_FILES
  src/skeleton/renderer/opengl/opengl_renderer.hpp
  src/skeleton/renderer/opengl/opengl_renderer.cpp
)
source_group("src/skeleton/renderer/opengl" FILES ${SKELETON_RENDERER_OPENGL_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_RENDERER_OPENGL_FILES}
)
