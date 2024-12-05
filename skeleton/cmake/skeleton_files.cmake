set(SKELETON_SRC_FILES
  include/skeleton/skeleton.hpp
  src/skeleton/skeleton.cpp
  include/skeleton/application_settings.hpp
  src/skeleton/platform.hpp
)
source_group("src/skeleton" FILES ${SKELETON_SRC_FILES})

set(SKELETON_SRC_WINDOW_FILES
  src/skeleton/window/window.hpp
  src/skeleton/window/window.cpp
)
source_group("src/skeleton/window" FILES ${SKELETON_SRC_WINDOW_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_SRC_FILES}
  ${SKELETON_SRC_WINDOW_FILES}
)
