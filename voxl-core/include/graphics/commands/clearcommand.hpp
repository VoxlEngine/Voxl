#ifndef VOXL_GRAPHICS_COMMANDS_CLEARCOMMAND_H_
#define VOXL_GRAPHICS_COMMANDS_CLEARCOMMAND_H_

#include "voxl.hpp"

#ifdef VOXL_API_VK
#include <vulkan/vulkan.h>
#elif defined(VOXL_API_GL)
#include <GL/gl.h>
#endif

#include "graphics/rendercommand.hpp"
#include "math/vec4.hpp"

namespace voxl {
namespace graphics {
namespace commands {
class ClearCommand : public RenderCommand {
public:
  ClearCommand(math::Vec4 clearColor);
  bool Execute(Context *context);

private:
  math::Vec4 color;
};
}
}
}

#endif
