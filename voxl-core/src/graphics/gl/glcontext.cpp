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

				window = glfwCreateWindow(config.windowWidth, config.windowHeight,
					config.windowTitle, nullptr, nullptr);

				if (window == nullptr) {
					std::cout << "Unable to create GLFW window" << std::endl;
					return false;
				}

				glViewport(0, 0, config.windowWidth, config.windowHeight);

				glfwMakeContextCurrent(window);

				return true;
			}

			void GlContext::Destroy() { glfwTerminate(); }

			void GlContext::StartFrame() {}

			void GlContext::EndFrame() { glfwSwapBuffers(window); }
		}
	}
}