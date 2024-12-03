set(SKELETON_CORE_SRC_FILES
  include/skeleton/skeleton.hpp
  include/skeleton/application_settings.hpp
  src/skeleton/skeleton.cpp
)
source_group("src/skeleton-core" FILES ${SKELETON_CORE_SRC_FILES})

set(SKELETON_CORE_SRC_WINDOW_FILES
  src/skeleton/window/window.hpp
  src/skeleton/window/window.cpp
)
source_group("src/skeleton-core/window" FILES ${SKELETON_CORE_SRC_WINDOW_FILES})

set(SKELETON_CORE_FILES
  ${SKELETON_CORE_FILES}
  ${SKELETON_CORE_SRC_FILES}
  ${SKELETON_CORE_SRC_WINDOW_FILES}
)
