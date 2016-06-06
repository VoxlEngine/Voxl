#ifndef VOXL_RENDERER_RENDERER_H_
#define VOXL_RENDERER_RENDERER_H_

namespace voxl {
namespace renderer {
class Renderer {
public:
  Renderer();
  ~Renderer();

private:
  VkInstance instance;
};
}
}

#endif
