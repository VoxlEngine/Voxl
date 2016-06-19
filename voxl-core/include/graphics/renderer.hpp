#ifndef VOXL_RENDERER_RENDERER_H_
#define VOXL_RENDERER_RENDERER_H_

#include "voxl.hpp"
#include <GLFW/glfw3.h>

#include "config.hpp"

#ifdef VOXL_API_VK
#include "graphics/vk/vkcontext.hpp"
#elif defined(VOXL_API_GL)
#include "graphics/gl/glcontext.hpp"
#endif

namespace voxl {
namespace graphics {
class Renderer {
public:
  Renderer(Config config);
  ~Renderer();

  void Render();

  inline GLFWwindow *getWindow() { return context.window; };

private:
#ifdef VOXL_API_VK
  vk::VkContext context;
#elif defined(VOXL_API_GL)
  gl::GlContext context;
#endif
};
}
}

#endif
