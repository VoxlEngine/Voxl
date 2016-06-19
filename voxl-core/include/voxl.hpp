#ifndef VOXL_VOXL_H_
#define VOXL_VOXL_H_

#define VOXL_VERSION_MAJOR 0
#define VOXL_VERSION_MINOR 0
#define VOXL_VERSION_PATCH 1

#define VOXL_USE_GL
//#define VOXL_USE_VULKAN

#ifdef _WIN32
#define VOXL_WINDOWS
#elif __linux__
#define VOXL_LINUX
#elif __ANDROID__
#define VOXL_ANDROID
#endif

#endif
