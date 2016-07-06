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

#include <cstring>
#include <iostream>
#include <vector>

#include "voxl.hpp"

#include "graphics/renderer.hpp"

namespace voxl {
	namespace graphics {
		Renderer::Renderer(voxl::Config config) {
#ifdef VOXL_API_VK
			context = new vk::VkContext();
#elif defined(VOXL_API_GL)
			context = new gl::GlContext();
#endif

			if (!context->Init(config)) {
				std::cout << "Unable to initialize context" << std::endl;
			}
		}
		
		Renderer::~Renderer() { context->Destroy(); }

		void Renderer::Render() {
			context->StartFrame();
			context->EndFrame();
		}
	}
}