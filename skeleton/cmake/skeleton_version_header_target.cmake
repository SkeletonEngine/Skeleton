
# Function to extract version information from Git
function(get_git_version VERSION_MAJOR VERSION_MINOR VERSION_PATCH)
  execute_process(
    COMMAND git describe --tags --abbrev=0
    RESULT_VARIABLE GIT_DESCRIBE_RESULT
    OUTPUT_VARIABLE LAST_TAG
  )

  string(REGEX MATCHALL "v([0-9]+)\\.([0-9]+)" MATCH "${LAST_TAG}")
  if(MATCH)
    set(${VERSION_MAJOR} "${CMAKE_MATCH_1}" PARENT_SCOPE)
    set(${VERSION_MINOR} "${CMAKE_MATCH_2}" PARENT_SCOPE)
  else()
    # Handle the case where no tags are found (e.g., set to 0.0)
    set(${VERSION_MAJOR} "0" PARENT_SCOPE)
    set(${VERSION_MINOR} "0" PARENT_SCOPE)
  endif()

  execute_process(
    COMMAND git rev-list --count "${MATCH}..HEAD"
    RESULT_VARIABLE GIT_COUNT_RESULT
    OUTPUT_VARIABLE COMMIT_COUNT
    ERROR_QUIET
  )
  string(STRIP "${COMMIT_COUNT}" COMMIT_COUNT)
  if(COMMIT_COUNT)
    math(EXPR PATCH "${COMMIT_COUNT}")
    set(${VERSION_PATCH} "${PATCH}" PARENT_SCOPE)
  else()
    set(${VERSION_PATCH} "0" PARENT_SCOPE)
  endif()
  
  message("The semantic version inferred from git tags and commit history is v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
endfunction()

# Display the version information
get_git_version(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# Define a custom command to generate the version header
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  COMMAND ${CMAKE_COMMAND} -E configure_file
    "${CMAKE_CURRENT_SOURCE_DIR}/include/skeleton/core/version.hpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/skeleton/core/version.hpp"
  VERBATIM
)

# Define a custom target for the version header
add_custom_target(skeleton-version-header ALL
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}"
)
