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

#ifndef VOXL_MATH_VEC2_H_
#define VOXL_MATH_VEC2_H_

namespace voxl {
	namespace math {
		class Vec2 {
		public:
			Vec2();
			Vec2(float xCoord, float yCoord);

			float x, y;

			Vec2 operator+(const float &scalar);
			Vec2 operator-(const float &scalar);

			Vec2 operator+(const Vec2 &other);
			Vec2 operator-(const Vec2 &other);

			Vec2 operator*(const float &scalar);
			Vec2 operator/(const float &scalar);

			Vec2 operator*(const Vec2 &other);
			Vec2 operator/(const Vec2 &other);

			float Distance();
			float Dot(Vec2 &other);
			Vec2 Negate();
		};
	}
}

#endif