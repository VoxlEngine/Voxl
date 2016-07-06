# Voxl Game Engine
# Copyright 2016 Sam H.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

project(voxl-core)

include_directories(include)

add_library(voxl
	# Sources
	src/engine/entity.cpp
	src/engine/entitymanager.cpp
	src/engine/game.cpp
	src/graphics/gl/glcontext.cpp
	src/graphics/renderer.cpp
	src/graphics/vk/util.cpp
	src/graphics/vk/vkcontext.cpp
	src/math/mat4.cpp
	src/math/vec2.cpp
	src/math/vec3.cpp
	src/math/vec4.cpp
	
	# Headers
	include/config.hpp
	include/engine/entity.hpp
	include/engine/entitymanager.hpp
	include/engine/game.hpp
	include/engine/scene.hpp
	include/graphics/context.hpp
	include/graphics/gl/glcontext.hpp
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