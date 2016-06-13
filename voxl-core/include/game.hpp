#ifndef VOXL_GAME_H_
#define VOXL_GAME_H_

#include "config.hpp"
#include "graphics/renderer.hpp"

namespace voxl {
class Game {
public:
  Game(Config config);

  void start();

private:
  graphics::Renderer renderer;
};
}

#endif
