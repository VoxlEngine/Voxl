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

#include <math.h>

#include "math/vec2.hpp"

namespace voxl {
	namespace math {
		Vec2::Vec2() {
			x = 0.0f;
			y = 0.0f;
		}

		Vec2::Vec2(float xCoord, float yCoord) {
			x = xCoord;
			y = yCoord;
		}

		Vec2 Vec2::operator+(const float &scalar) {
			return Vec2(x + scalar, y + scalar);
		}

		Vec2 Vec2::operator-(const float &scalar) {
			return Vec2(x - scalar, y - scalar);
		}

		Vec2 Vec2::operator+(const Vec2 &other) {
			return Vec2(x + other.x, y + other.y);
		}

		Vec2 Vec2::operator-(const Vec2 &other) {
			return Vec2(x - other.x, y - other.y);
		}

		Vec2 Vec2::operator*(const float &scalar) {
			return Vec2(x * scalar, y * scalar);
		}

		Vec2 Vec2::operator/(const float &scalar) {
			return Vec2(x / scalar, y / scalar);
		}

		Vec2 Vec2::operator*(const Vec2 &other) {
			return Vec2(x * other.x, y * other.y);
		}

		Vec2 Vec2::operator/(const Vec2 &other) {
			return Vec2(x / other.x, y / other.y);
		}

		float Vec2::Distance() { return (float)sqrt(x * x + y * y); }

		float Vec2::Dot(Vec2 &other) { return x * other.x + y * other.y; }

		Vec2 Vec2::Negate() { return Vec2(-x, -y); }
	}
}