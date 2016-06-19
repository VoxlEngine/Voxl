project(voxl-core)

add_library(voxl
  src/app/game.cpp
  src/graphics/gl/glcontext.cpp
  src/graphics/vk/util.cpp
  src/graphics/vk/vkcontext.cpp
  src/graphics/rendercommand.cpp
  src/graphics/renderer.cpp
  src/math/mat4.cpp
  src/math/vec2.cpp
  src/math/vec3.cpp
  src/math/vec4.cpp)
