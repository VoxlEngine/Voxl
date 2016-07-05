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

#include "math/vec4.hpp"

namespace voxl {
	namespace math {
		Vec4::Vec4() {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}

		Vec4::Vec4(float xCoord, float yCoord, float zCoord, float wCoord) {
			x = xCoord;
			y = yCoord;
			z = zCoord;
			w = wCoord;
		}

		Vec4 Vec4::operator+(const float &scalar) {
			return Vec4(x + scalar, y + scalar, z + scalar, w + scalar);
		}

		Vec4 Vec4::operator-(const float &scalar) {
			return Vec4(x - scalar, y - scalar, z - scalar, w - scalar);
		}

		Vec4 Vec4::operator+(const Vec4 &other) {
			return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vec4 Vec4::operator-(const Vec4 &other) {
			return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vec4 Vec4::operator*(const float &scalar) {
			return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		Vec4 Vec4::operator/(const float &scalar) {
			return Vec4(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		Vec4 Vec4::operator*(const Vec4 &other) {
			return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
		}

		Vec4 Vec4::operator/(const Vec4 &other) {
			return Vec4(x / other.x, y / other.y, z / other.z, w / other.w);
		}

		float Vec4::Distance() { return (float)sqrt(x * x + y * y + z * z + w * w); }

		float Vec4::Dot(Vec4 &other) {
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		Vec4 Vec4::Negate() { return Vec4(-x, -y, -z, -w); }
	}
}