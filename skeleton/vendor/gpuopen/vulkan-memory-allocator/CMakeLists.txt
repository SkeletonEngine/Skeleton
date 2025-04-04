cmake_minimum_required(VERSION 3.21)

add_subdirectory(vulkan-memory-allocator)

add_library(vulkan-memory-allocator vma_implementation.cpp)
target_link_libraries(vulkan-memory-allocator PRIVATE VulkanMemoryAllocator)
target_link_libraries(vulkan-memory-allocator PRIVATE volk)
target_include_directories(vulkan-memory-allocator PUBLIC vulkan-memory-allocator/include)
target_compile_definitions(vulkan-memory-allocator PRIVATE VMA_STATIC_VULKAN_FUNCTIONS=0 VMA_DYNAMIC_VULKAN_FUNCTIONS=0)
