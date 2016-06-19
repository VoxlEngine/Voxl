#include <iostream>

#include <voxl.hpp>

#include <app/game.hpp>
#include <config.hpp>

using namespace voxl;
using namespace voxl::app;

int main() {
  Config config;
  config.windowWidth = 1336;
  config.windowHeight = 768;
  config.windowTitle = "Voxl Example";

  Game game(config);
  game.start();
  return 0;
}
