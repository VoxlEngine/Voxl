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

#pragma once

namespace voxl {
	namespace math {
		class Vec4 {
		public:
			Vec4();
			Vec4(float xCoord, float yCoord, float zCoord, float wCoord);

			float x, y, z, w;

			Vec4 operator+(const float &scalar);
			Vec4 operator-(const float &scalar);

			Vec4 operator+(const Vec4 &other);
			Vec4 operator-(const Vec4 &other);

			Vec4 operator*(const float &scalar);
			Vec4 operator/(const float &scalar);

			Vec4 operator*(const Vec4 &other);
			Vec4 operator/(const Vec4 &other);

			float Distance();
			float Dot(Vec4 &other);
			Vec4 Negate();
		};
	}
}
