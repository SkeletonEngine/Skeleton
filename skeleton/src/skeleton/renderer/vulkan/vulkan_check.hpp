// Copyright 2024-2025 SkeletonEngine

#pragma once

#include <cstdio>
#include <volk.h>
#include <vulkan/vk_enum_string_helper.h>

#ifndef NDEBUG

#define VK_CHECK(x) { \
  VkResult result = (x); \
  if (result != VK_SUCCESS) { \
    fprintf(stderr, "VK_CHECK failed: Call to %s (%s:%d) returned %s\n", \
      #x, __FILE__, __LINE__, string_VkResult(result)); \
    SK_DEBUGBREAK(); \
  } \
}

#else

#define VK_CHECK(x) x

#endif
