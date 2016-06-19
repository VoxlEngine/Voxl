#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/renderer.hpp"

namespace voxl {
namespace graphics {

Renderer::Renderer(Config config) {
  if (!context.Init(config)) {
    std::cout << "Unable to initialize context" << std::endl;
  }
}
Renderer::~Renderer() { context.Destroy(); }

void Renderer::Render() { context.Swap(); }
}
}
