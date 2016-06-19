#ifndef VOXL_GAME_H_
#define VOXL_GAME_H_

#include "voxl.hpp"

#include "config.hpp"
#include "graphics/renderer.hpp"

namespace voxl {
namespace app {
class Game {
public:
  Game(Config config);

  void start();

private:
  graphics::Renderer renderer;

  int frameCount;
};
}
}

#endif
