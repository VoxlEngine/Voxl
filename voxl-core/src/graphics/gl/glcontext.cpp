#include <cstring>
#include <iostream>
#include <vector>

#include <assert.h>

#include "graphics/gl/glcontext.hpp"

namespace voxl {
namespace graphics {
namespace gl {
bool GlContext::Init(Config config) {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "Unable to initialize GLFW" << std::endl;
  }

  // Create window
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(config.windowWidth, config.windowHeight,
                            config.windowTitle, NULL, NULL);

  if (window == NULL) {
    std::cout << "Unable to create GLFW window" << std::endl;
  }
}

void GlContext::Destroy() { glfwTerminate(); }

void GlContext::Swap() {}
}
}
}
