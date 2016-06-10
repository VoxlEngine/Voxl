#include "game.hpp"

namespace voxl {
Game::Game(Config config) : renderer(config) {}

void Game::start() {
  while (!glfwWindowShouldClose(renderer.getWindow())) {
    glfwPollEvents();
  }
}
}
