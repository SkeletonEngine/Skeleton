set(SKELETON_SRC_FILES
  include/skeleton/skeleton.hpp
  src/skeleton/skeleton.cpp
)
source_group("src/skeleton" FILES ${SKELETON_SRC_FILES})

set(SKELETON_SRC_CORE_FILES
  include/skeleton/core/assert.hpp
  include/skeleton/core/build_configuration.hpp
  include/skeleton/core/core.hpp
  include/skeleton/core/platform.hpp
  include/skeleton/core/version.hpp
)
source_group("src/skeleton/core" FILES ${SKELETON_SRC_CORE_FILES})

set(SKELETON_SRC_WINDOW_FILES
  include/skeleton/window/window.hpp
  include/skeleton/window/window_backend.hpp
)
source_group("src/skeleton/window" FILES ${SKELETON_SRC_WINDOW_FILES})

set(SKELETON_SRC_WINDOW_GLFW_FILES
  include/skeleton/window/glfw/glfw_window.hpp
  src/skeleton/window/glfw/glfw_window.cpp
)
source_group("src/skeleton/window/glfw" FILES ${SKELETON_SRC_WINDOW_GLFW_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_SRC_FILES}
  ${SKELETON_SRC_CORE_FILES}
  ${SKELETON_SRC_WINDOW_FILES}
  ${SKELETON_SRC_WINDOW_GLFW_FILES}
)
