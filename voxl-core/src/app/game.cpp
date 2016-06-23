#include "app/game.hpp"

#include <iostream>

#include "graphics/commands/clearcommand.hpp"
#include "math/vec4.hpp"

namespace voxl {
namespace app {
Game::Game(Config config) : renderer(config) {
  renderer.Submit(
      new graphics::commands::ClearCommand(math::Vec4(0.3f, 0.3f, 0.3f, 1.0f)));
}

void Game::start() {
  float lastTime = glfwGetTime();

  while (!glfwWindowShouldClose(renderer.getWindow())) {
    float curTime = glfwGetTime();
    float delta = curTime - lastTime;
    glfwPollEvents();
    lastTime = curTime;

    renderer.Render();
    // std::cout << "FPS: " << 1000.0f / (delta * 1000.0f) << std::endl;
  }
}
}
}
