project(voxl-core)

add_library(voxl
  src/math/mat4.cpp
  src/math/vec2.cpp
  src/math/vec3.cpp
  src/math/vec4.cpp
  src/graphics/vulkan/vulkancontext.cpp
  src/graphics/renderer.cpp
  src/game.cpp)
