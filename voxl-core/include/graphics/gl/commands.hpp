#ifndef VOXL_GRAPHICS_GL_COMMANDS_H_
#define VOXL_GRAPHICS_GL_COMMANDS_H_

#include "graphics/rendercommand.hpp"
#include "math/vec4.hpp"

namespace voxl {
namespace graphics {
namespace gl {
class GlClearCommand : public RenderCommand {
public:
  GlClearCommand(math::Vec4 clearColor);
  bool Execute(Context &context);

private:
  math::Vec4 color;
};
}
}
}

#endif
