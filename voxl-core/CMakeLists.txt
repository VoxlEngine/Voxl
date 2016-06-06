project(voxl-core)

find_package(Vulkan REQUIRED)

add_library(voxl
  src/math/mat4.cpp
  src/math/vec2.cpp
  src/math/vec3.cpp
  src/math/vec4.cpp
  src/renderer/renderer.cpp)

include_directories(voxl ${VULKAN_INCLUDE_DIR})
