#include <cstring>
#include <iostream>
#include <vector>

#include "graphics/renderer.hpp"

namespace voxl {
namespace graphics {

Renderer::Renderer(Config config) { context.Init(config); }
Renderer::~Renderer() { context.Destroy(); }

void Renderer::Update() { context.Swap(); }
}
}
