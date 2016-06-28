project(voxl-core)

include_directories(include)

add_library(voxl
	# Sources
	src/app/game.cpp
	src/graphics/commands/clearcommand.cpp
	src/graphics/gl/glcontext.cpp
	src/graphics/renderer.cpp
	src/graphics/vk/util.cpp
	src/graphics/vk/vkcontext.cpp
	src/math/mat4.cpp
	src/math/vec2.cpp
	src/math/vec3.cpp
	src/math/vec4.cpp
	
	# Headers
	include/app/game.hpp
	include/config.hpp
	include/graphics/commands/clearcommand.hpp
	include/graphics/context.hpp
	include/graphics/gl/glcontext.hpp
	include/graphics/rendercommand.hpp
	include/graphics/renderer.hpp
	include/graphics/vk/util.hpp
	include/graphics/vk/vkcontext.hpp
	include/math/mat4.hpp
	include/math/math.hpp
	include/math/vec2.hpp
	include/math/vec3.hpp
	include/math/vec4.hpp
	include/types.hpp
	include/voxl.hpp
	)