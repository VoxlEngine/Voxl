#include "game.hpp"

#include <iostream>

#ifndef VOXL_LINUX
#include <unistd.h>
#endif

#ifdef VOXL_WINDOWS
#include <windows.h>
#endif

namespace voxl {
Game::Game(Config config) : renderer(config) {}

void Game::start() {
  float sync = 0.0166f;

  float lastTime = glfwGetTime();

  while (!glfwWindowShouldClose(renderer.getWindow())) {
    float curTime = glfwGetTime();
    float delta = curTime - lastTime;

    if (delta >= sync) {
      glfwPollEvents();

      lastTime = curTime;

      renderer.Update();

      std::cout << "FPS: " << 1000.0f / (delta * 1000.0f) << std::endl;
    } else {
#ifndef VOXL_LINUX
      usleep((sync - delta) * 1000);
#endif
#ifdef VOXL_WINDOWS
      Sleep((sync - delta));
#endif
    }
  }
}
}
