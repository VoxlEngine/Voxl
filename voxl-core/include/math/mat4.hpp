#ifndef VOXL_MATH_MAT4_H_
#define VOXL_MATH_MAT4_H_

#include "types.hpp"

#include "math/vec3.hpp"
#include "math/vec4.hpp"

namespace voxl {
	namespace math {
		class Mat4 {
		public:
			Mat4(float diagonal);

			inline float Get(uint i, uint j) { return elements[i + j * 4]; };
			inline float *GetElements() { return elements; };
			inline void Set(uint i, uint j, float val) { elements[i + j * 4] = val; };

			Mat4 operator*(Mat4 &other);
			Vec4 operator*(Vec4 &other);
			void operator*=(Mat4 &other);

			static Mat4 Translate(Vec3 delta);

			static Mat4 RotateX(float radians);
			static Mat4 RotateY(float radians);
			static Mat4 RotateZ(float radians);

			static Mat4 LookAt(Vec3 right, Vec3 up, Vec3 dir, Vec3 pos);

			static Mat4 Ortho(float left, float right, float bottom, float top,
				float near, float far);
			static Mat4 Persp(float fov, float aspectRatio, float near, float far);

			void Print();

		private:
			float elements[16];
		};
	}
}

#endif