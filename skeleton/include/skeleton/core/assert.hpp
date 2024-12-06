#pragma once

#include "skeleton/core/build_configuration.hpp"
#include "skeleton/core/platform.hpp"

#if defined(SK_PLATFORM_WINDOWS)
  #define SK_DEBUGBREAK() __debugbreak()
#elif defined(SK_PLATFORM_LINUX) || defined(SK_PLATFORM_MACOS)
  #include <signal.h>
  #define SK_DEBUGBREAK() raise(SIGTRAP)
#else
  #error "Unable to deduce SK_DEBUGBREAK implementation for platform"
#endif

#ifdef SK_BUILD_DEBUG
  #define SK_ASSERT(x) if (!(x)) { SK_DEBUGBREAK(); }
  #define SK_CHECK(x)  SK_ASSERT(x)
#else
  #define SK_ASSERT(x)
  #define SK_CHECK(x)  x
#endif
