#pragma once

#ifdef _WIN32
  #define SK_PLATFORM_WINDOWS

#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "iOS simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
		#error "iOS is not supported"
	#elif TARGET_OS_MAC == 1
		#define SK_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform"
	#endif

#elif defined(__ANDROID__)
	#error "Android is not supported"

#elif defined(__linux__)
	#define SK_PLATFORM_LINUX

#else
	#error "Unable to detect platform"
#endif
