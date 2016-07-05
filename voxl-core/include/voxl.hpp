/*
* Voxl Game Engine
* Copyright 2016 Sam H.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef VOXL_VOXL_H_
#define VOXL_VOXL_H_

// Voxl Engine version
#define VOXL_VERSION_MAJOR 0
#define VOXL_VERSION_MINOR 0
#define VOXL_VERSION_PATCH 15

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
#if defined(_DEBUG)
#define VOXL_DEBUG
#endif


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