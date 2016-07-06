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

#pragma once

#include "voxl.hpp"
#include <GLFW/glfw3.h>
#include <vector>

#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 3
#include <GL/gl.h>

#include "config.hpp"

#include "graphics/context.hpp"

namespace voxl {
	namespace graphics {
		namespace gl {
			class GlContext : public Context {
			public:
				bool Init(Config config);
				void Destroy();
				void StartFrame();
				void EndFrame();

				inline GLFWwindow *GetWindow() { return window; };

			private:
				GLFWwindow *window;
			};
		}
	}
}
