#pragma once

#include <cstdio>
#include <volk.h>

#ifndef NDEBUG

namespace Skeleton {

const char* VkResultString(VkResult result);

}

#define VK_CHECK(x) { \
  VkResult result = (x); \
  if (result != VK_SUCCESS) { \
fprintf(stderr, "VK_CHECK failed: %s => %s\n", #x, ::Skeleton::VkResultString(result)); \
  } \
}

#else

#define VK_CHECK(x) x

#endif
