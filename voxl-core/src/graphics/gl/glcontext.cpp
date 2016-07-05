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

#include <assert.h>

#include "graphics/gl/glcontext.hpp"

namespace voxl {
	namespace graphics {
		namespace gl {
			bool GlContext::Init(Config config) {
				// Initialize GLFW
				if (!glfwInit()) {
					std::cout << "Unable to initialize GLFW" << std::endl;
					return false;
				}

				// Create window
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
				glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
				glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

				window = glfwCreateWindow(config.windowWidth, config.windowHeight,
					config.windowTitle, nullptr, nullptr);

				if (window == nullptr) {
					std::cout << "Unable to create GLFW window" << std::endl;
					return false;
				}

				glViewport(0, 0, config.windowWidth, config.windowHeight);

				glfwMakeContextCurrent(window);

				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				return true;
			}

			void GlContext::Destroy() { glfwTerminate(); }

			void GlContext::StartFrame() {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			void GlContext::EndFrame() { glfwSwapBuffers(window); }
		}
	}
}