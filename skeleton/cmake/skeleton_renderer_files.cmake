set(SKELETON_RENDERER_FILES
  include/skeleton/renderer/renderer_settings.hpp
  include/skeleton/renderer/renderer_backend.hpp
  include/skeleton/renderer/renderer.hpp
)
source_group("src/skeleton/renderer" FILES ${SKELETON_RENDERER_FILES})

set(SKELETON_RENDERER_COMMON_FILES
  src/skeleton/renderer/common/spv_file.hpp
  src/skeleton/renderer/common/spv_file.cpp
)
source_group("src/skeleton/renderer/common" FILES ${SKELETON_RENDERER_COMMON_FILES})

set(SKELETON_RENDERER_COMMON_SHADER_REFLECTION_FILES
  src/skeleton/renderer/common/shader_reflection/shader_data_type.hpp
  src/skeleton/renderer/common/shader_reflection/shader_data_type.cpp
  src/skeleton/renderer/common/shader_reflection/shader_buffer_element.hpp
  src/skeleton/renderer/common/shader_reflection/shader_buffer_layout.hpp
  src/skeleton/renderer/common/shader_reflection/shader_buffer_layout.cpp
  src/skeleton/renderer/common/shader_reflection/shader_reflection_details.hpp
  src/skeleton/renderer/common/shader_reflection/shader_reflection_details.cpp
)
source_group("src/skeleton/renderer/common/shader_reflection" FILES ${SKELETON_RENDERER_COMMON_SHADER_REFLECTION_FILES})

set(SKELETON_FILES
  ${SKELETON_FILES}
  ${SKELETON_RENDERER_FILES}
  ${SKELETON_RENDERER_COMMON_FILES}
  ${SKELETON_RENDERER_COMMON_SHADER_REFLECTION_FILES}
)
