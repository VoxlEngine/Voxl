#ifndef VOXL_VOXL_H_
#define VOXL_VOXL_H_

#define VOXL_VERSION_MAJOR 0
#define VOXL_VERSION_MINOR 0
#define VOXL_VERSION_PATCH 1

#define VOXL_USE_VULKAN

#ifdef _WIN32
#define VOXL_WINDOWS
#elif __linux__
#define VOXL_LINUX
#elif __ANDROID__
#define VOXL_ANDROID
#endif

#ifdef VOXL_USE_VULKAN

#define VULKAN_VERSION_MAJOR 1
#define VULKAN_VERSION_MINOR 0
#define VULKAN_VERSION_PATCH 1

#define VK_PROTOTYPES

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#else
#define VK_USE_PLATFORM_XCB_KHR
#endif

#include <vulkan/vulkan.h>

#endif

#define ENGINE_NAME "Voxl"

#endif
