#ifndef VOXL_VOXL_H_
#define VOXL_VOXL_H_

#define VOXL_VERSION_MAJOR 0
#define VOXL_VERSION_MINOR 0
#define VOXL_VERSION_PATCH 1

#define VOXL_USE_VULKAN

#ifdef VOXL_USE_VULKAN

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 1

#include <vulkan/vulkan.h>

#endif

#define ENGINE_NAME "Voxl"

#endif
