# Define a custom target that will generate the version header
add_custom_target(skeleton-version-header
  cmake -DSRCDIR=${CMAKE_SOURCE_DIR} -P "${CMAKE_SOURCE_DIR}/skeleton/cmake/skeleton_version_header_command.cmake" 
)