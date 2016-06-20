#include "app/game.hpp"

#include <iostream>

#ifndef VOXL_LINUX
#include <unistd.h>
#endif

#ifdef VOXL_WINDOWS
#include <windows.h>
#endif

#include "graphics/commands/clearcommand.hpp"
#include "math/vec4.hpp"

namespace voxl {
namespace app {
Game::Game(Config config) : renderer(config) {}

void Game::start() {
  float lastTime = glfwGetTime();

  while (!glfwWindowShouldClose(renderer.getWindow())) {
    float curTime = glfwGetTime();
    float delta = curTime - lastTime;
    glfwPollEvents();
    lastTime = curTime;

    graphics::commands::ClearCommand clearCmd(
        math::Vec4(0.3f, 0.3f, 0.3f, 1.0f));
    renderer.Submit(&clearCmd);

    renderer.Render();
    std::cout << "FPS: " << 1000.0f / (delta * 1000.0f) << std::endl;
  }
}
}
}
