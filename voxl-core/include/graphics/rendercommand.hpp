#ifndef VOXL_GRAPHICS_RENDERCOMMAND_H_
#define VOXL_GRAPHICS_RENDERCOMMAND_H_

#include "graphics/context.hpp"

namespace voxl {
namespace graphics {
class RenderCommand {
public:
  virtual bool Execute(Context &context) = 0;
};
}
}

#endif
