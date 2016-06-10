#include <iostream>

#include <voxl.hpp>

#include <config.hpp>
#include <game.hpp>

using namespace voxl;
using namespace voxl::renderer;

int main() {
  Config config;
  config.windowWidth = 1336;
  config.windowHeight = 768;
  config.windowTitle = "Voxl Example";

  Game game(config);
  game.start();
  return 0;
}
