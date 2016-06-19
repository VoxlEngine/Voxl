#ifndef VOXL_GRAPHICS_GL_CONTEXT_H_
#define VOXL_GRAPHICS_GL_CONTEXT_H_

#define GL_VERSION_MAJOR 1
#define GL_VERSION_MINOR 0
#define GL_VERSION_PATCH 8
#include <GL/gl.h>

#include "voxl.hpp"
#include <GLFW/glfw3.h>
#include <vector>

#include "config.hpp"

#include "graphics/context.hpp"

namespace voxl {
namespace graphics {
namespace gl {
class GlContext : public Context {
public:
  bool Init(Config config);
  void Destroy();
  void Swap();

  GLFWwindow *window;
};
}
}
}

#endif
