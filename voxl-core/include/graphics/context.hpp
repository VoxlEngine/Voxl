#ifndef VOXL_GRAPHICS_CONTEXT_H_
#define VOXL_GRAPHICS_CONTEXT_H_

#include "config.hpp"

namespace voxl {
namespace graphics {
class Context {
public:
  virtual bool Init(Config config) = 0;
  virtual void Destroy() = 0;
  virtual void StartFrame() = 0;
  virtual void EndFrame() = 0;
};
}
}

#endif
