#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/renderer.hpp"

namespace voxl {
namespace graphics {

Renderer::Renderer(Config config) { vulkanContext.Init(config); }
Renderer::~Renderer() { vulkanContext.Destroy(); }

void Renderer::Update() { vulkanContext.Swap(); }
}
}
