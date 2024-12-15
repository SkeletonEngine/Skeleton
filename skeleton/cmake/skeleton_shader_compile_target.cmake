file(GLOB_RECURSE GLSL_SOURCE_FILES "src/skeleton/shaders/*.glsl")

set(SPIRV_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/shaders/")
foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(STEM ${GLSL} NAME_WLE)
  set(SPIRV "${SPIRV_OUTPUT_DIR}${STEM}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SPIRV_OUTPUT_DIR}"
    COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL} ${GLSL_HEADER_FILES})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach()

add_custom_target(skeleton-shaders DEPENDS ${SPIRV_BINARY_FILES} SOURCES ${GLSL_SOURCE_FILES})
