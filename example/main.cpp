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

#include <iostream>

#include <voxl.hpp>

using namespace voxl;
using namespace voxl::engine;

int main() {
	Config config;
	config.windowWidth = 1336;
	config.windowHeight = 768;
	config.windowTitle = "Voxl Example";

	Scene scene;
	Entity* entity = scene.GetEntityManager()->CreateEntity();
	scene.GetEntityManager()->DeleteEntity(entity);

	Game game(config);
	game.start();
	return 0;
}