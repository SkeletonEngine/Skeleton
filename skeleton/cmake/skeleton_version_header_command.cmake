# This whole file is a scriptable command that will generate the version.hpp file from a template based on git tags/commits
# First we look for the most recent tag and extract the major/minor version from that
# e.g. if the most recent tag in the current branch is v1.2 we set the major/minor version to 1 and 2 respectively
# After that we count the number of commits since the most recent tag. That's our patch version.

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
endfunction()

get_git_version(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)
message("The semantic version inferred from git tags and commit history is v${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if (NOT DEFINED SRCDIR)
  message("SRCDIR is not defined, using CMAKE_SOURCE_DIR instead")
  set(SRCDIR ${CMAKE_SOURCE_DIR})
endif()

configure_file(
  "${SRCDIR}/skeleton/include/skeleton/core/version.hpp.in"
  "${SRCDIR}/skeleton/include/skeleton/core/version.hpp"
)
