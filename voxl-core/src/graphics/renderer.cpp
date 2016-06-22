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

  context.StartFrame();
  context.EndFrame();
}
Renderer::~Renderer() { context.Destroy(); }

void Renderer::Render() {
  context.StartFrame();

  // Run all render commands
  for (int i = 0; i < commands.size(); i++) {
    commands[i]->Execute(&context);
    commands.erase(commands.begin() + i);
  }

  context.EndFrame();
}

void Renderer::Submit(RenderCommand *command) { commands.push_back(command); }
void Renderer::ClearCommands() { commands.clear(); }
}
}
