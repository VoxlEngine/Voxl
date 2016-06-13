#include <math.h>

#include "math/vec3.hpp"

namespace voxl {
namespace math {
Vec3::Vec3() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

Vec3::Vec3(float xCoord, float yCoord, float zCoord) {
  x = xCoord;
  y = yCoord;
  z = zCoord;
}

Vec3 Vec3::operator+(const float &scalar) {
  return Vec3(x + scalar, y + scalar, z + scalar);
}

Vec3 Vec3::operator-(const float &scalar) {
  return Vec3(x - scalar, y - scalar, z - scalar);
}

Vec3 Vec3::operator+(const Vec3 &other) {
  return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3 &other) {
  return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(const float &scalar) {
  return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(const float &scalar) {
  return Vec3(x / scalar, y / scalar, z / scalar);
}

Vec3 Vec3::operator*(const Vec3 &other) {
  return Vec3(x * other.x, y * other.y, z * other.z);
}

Vec3 Vec3::operator/(const Vec3 &other) {
  return Vec3(x / other.x, y / other.y, z / other.z);
}

float Vec3::Distance() { return sqrt(x * x + y * y + z * z); }

float Vec3::Dot(Vec3 &other) { return x * other.x + y * other.y + z * other.z; }

Vec3 Vec3::Cross(Vec3 &other) {
  return Vec3(y * other.z - z * other.y, z * other.x - x * other.z,
              x * other.y - y * other.x);
}

Vec3 Vec3::Negate() { return Vec3(-x, -y, -z); }

Vec3 Vec3::Normalize() {
  Vec3 ret = *this;
  float length = ret.Distance();

  ret.x /= length;
  ret.y /= length;
  ret.z /= length;

  return ret;
}

Vec3 Vec3::Normalize(Vec3 vec) {
  float length = vec.Distance();

  vec.x /= length;
  vec.y /= length;
  vec.z /= length;

  return vec;
}
}
}
