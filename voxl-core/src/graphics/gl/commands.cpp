#include <GL/gl.h>

#include <iostream>

#include "graphics/gl/commands.hpp"

namespace voxl {
namespace graphics {
namespace gl {
GlClearCommand::GlClearCommand(math::Vec4 clearColor) : color(clearColor) {}
bool GlClearCommand::Execute(Context &context) {
  glClearColor(0.3f, 0.4f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return true;
}
}
}
}
