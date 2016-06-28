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