#include <math.h>

#include "math/vec4.hpp"

namespace voxl {
namespace math {
vec4::vec4() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
  w = 0.0f;
}

vec4::vec4(float xCoord, float yCoord, float zCoord, float wCoord) {
  x = xCoord;
  y = yCoord;
  z = zCoord;
  w = wCoord;
}

vec4 vec4::operator+(const float &scalar) {
  return vec4(x + scalar, y + scalar, z + scalar, w + scalar);
}

vec4 vec4::operator-(const float &scalar) {
  return vec4(x - scalar, y - scalar, z - scalar, w - scalar);
}

vec4 vec4::operator+(const vec4 &other) {
  return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
}

vec4 vec4::operator-(const vec4 &other) {
  return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
}

vec4 vec4::operator*(const float &scalar) {
  return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

vec4 vec4::operator/(const float &scalar) {
  return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
}

vec4 vec4::operator*(const vec4 &other) {
  return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
}

vec4 vec4::operator/(const vec4 &other) {
  return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
}

float vec4::distance() { return sqrt(x * x + y * y + z * z + w * w); }

float vec4::dot(vec4 &other) {
  return x * other.x + y * other.y + z * other.z + w * other.w;
}

vec4 vec4::negate() { return vec4(-x, -y, -z, -w); }
}
}
