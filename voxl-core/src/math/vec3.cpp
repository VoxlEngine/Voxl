#include <math.h>

#include "math/vec3.hpp"

namespace voxl {
namespace math {
vec3::vec3() {
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

vec3::vec3(float xCoord, float yCoord, float zCoord) {
  x = xCoord;
  y = yCoord;
  z = zCoord;
}

vec3 vec3::operator+(const float &scalar) {
  return vec3(x + scalar, y + scalar, z + scalar);
}

vec3 vec3::operator-(const float &scalar) {
  return vec3(x - scalar, y - scalar, z - scalar);
}

vec3 vec3::operator+(const vec3 &other) {
  return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator-(const vec3 &other) {
  return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator*(const float &scalar) {
  return vec3(x * scalar, y * scalar, z * scalar);
}

vec3 vec3::operator/(const float &scalar) {
  return vec3(x / scalar, y / scalar, z / scalar);
}

vec3 vec3::operator*(const vec3 &other) {
  return vec3(x * other.x, y * other.y, z * other.z);
}

vec3 vec3::operator/(const vec3 &other) {
  return vec3(x / other.x, y / other.y, z / other.z);
}

float vec3::distance() { return sqrt(x * x + y * y + z * z); }

float vec3::dot(vec3 &other) { return x * other.x + y * other.y + z * other.z; }

vec3 vec3::cross(vec3 &other) {
  return vec3(y * other.z - z * other.y, z * other.x - x * other.z,
              x * other.y - y * other.x);
}

vec3 vec3::negate() { return vec3(-x, -y, -z); }

vec3 vec3::normalize() {
  vec3 ret = *this;
  float length = ret.distance();

  ret.x /= length;
  ret.y /= length;
  ret.z /= length;

  return ret;
}

vec3 vec3::normalize(vec3 vec) {
  float length = vec.distance();

  vec.x /= length;
  vec.y /= length;
  vec.z /= length;

  return vec;
}
}
}
