// Copyright 2024-2025 SkeletonEngine

#pragma once

// Volk has to be included before VMA so VMA doesn't try to include vulkan.h
#include <volk.h>
#include <vk_mem_alloc.h>
#include "skeleton/core/core.hpp"
#include "skeleton/renderer/vulkan/vulkan_check.hpp"

#define SK_VK_API_VERSION VK_API_VERSION_1_0

#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
