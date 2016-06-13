#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/renderer.hpp"

namespace voxl {
namespace graphics {

Renderer::Renderer(Config config) {
  graphicsContext.Init(config);
  graphicsContext.Swap();
}
Renderer::~Renderer() { graphicsContext.Destroy(); }

void Renderer::Update() {}
}
}
