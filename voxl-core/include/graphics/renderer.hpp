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

#ifndef VOXL_RENDERER_RENDERER_H_
#define VOXL_RENDERER_RENDERER_H_

#include "voxl.hpp"
#include <GLFW/glfw3.h>
#include <vector>

#include "config.hpp"

#include "graphics/context.hpp"

#ifdef VOXL_API_VK
#include "graphics/vk/vkcontext.hpp"
#elif defined(VOXL_API_GL)
#include "graphics/gl/glcontext.hpp"
#endif

namespace voxl {
	namespace graphics {
		class Renderer {
		public:
			Renderer(voxl::Config config);
			~Renderer();

			void Render();

			inline GLFWwindow *GetWindow() { return context->GetWindow(); };

		private:
			Context* context;
		};
	}
}

#endif