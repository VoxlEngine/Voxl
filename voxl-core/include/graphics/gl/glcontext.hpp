#ifndef VOXL_GRAPHICS_GL_CONTEXT_H_
#define VOXL_GRAPHICS_GL_CONTEXT_H_

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

				GLFWwindow *window;
			};
		}
	}
}

#endif