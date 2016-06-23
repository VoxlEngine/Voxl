#ifndef VOXL_VOXL_H_
#define VOXL_VOXL_H_

// Voxl Engine version
#define VOXL_VERSION_MAJOR 0
#define VOXL_VERSION_MINOR 0
#define VOXL_VERSION_PATCH 7

// Graphics API definitions
// Make sure that one of these if always defined/uncommented, and
// that all other definitions are undefined/commented

// Use OpenGL for graphics
#define VOXL_API_GL

// Use Vulkan for graphics
// #define VOXL_API_VK

// Debug
// This will enable/disable various features such as debug logging, Vulkan
// validation layers, etc.
#if defined(_DEBUG) || defined(DEBUG) || defined(NDEBUG)
#define VOXL_DEBUG
#endif

// DISABLE THIS!!!!
#define VOXL_DEBUG

// Platform definitions
// This controls platform-specific components of the engine
#ifdef _WIN32
#define VOXL_WINDOWS
#elif defined(__linux__)
#define VOXL_LINUX
#elif defined(__ANDROID__)
#define VOXL_ANDROID
#endif

// Standard includes
#ifdef VOXL_LINUX
#include <unistd.h>
#endif

#ifdef VOXL_WINDOWS
#include <windows.h>
#endif


#endif
