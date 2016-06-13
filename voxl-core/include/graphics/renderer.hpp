#ifndef VOXL_RENDERER_RENDERER_H_
#define VOXL_RENDERER_RENDERER_H_

#include "voxl.hpp"
#include <GLFW/glfw3.h>

#include "config.hpp"
#include "graphics/graphics_context.hpp"

namespace voxl {
namespace graphics {
class Renderer {
public:
  Renderer(Config config);
  ~Renderer();

  void Update();

  inline GLFWwindow *getWindow() { return graphicsContext.window; };

private:
  GraphicsContext graphicsContext;
};
}
}

#endif
