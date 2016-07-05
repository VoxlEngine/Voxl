/*
* Voxl Game Engine
* Copyright 2016 Sam H.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "app/game.hpp"

#include <iostream>

#include "math/vec4.hpp"

namespace voxl {
	namespace app {
		Game::Game(Config config) : renderer(config) {}

		void Game::start() {
			double lastTime = glfwGetTime();

			while (!glfwWindowShouldClose(renderer.GetWindow())) {
				double curTime = glfwGetTime();
				double delta = curTime - lastTime;
				glfwPollEvents();
				lastTime = curTime;

				renderer.Render();
				std::cout << "FPS: " << 1000.0f / (delta * 1000.0f) << std::endl;
			}
		}
	}
}