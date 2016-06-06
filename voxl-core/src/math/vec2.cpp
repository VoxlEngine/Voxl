#include <math.h>

#include "math/vec2.hpp"

namespace voxl {
namespace math {
vec2::vec2() {
  x = 0.0f;
  y = 0.0f;
}

vec2::vec2(float xCoord, float yCoord) {
  x = xCoord;
  y = yCoord;
}

vec2 vec2::operator+(const float &scalar) {
  return vec2(x + scalar, y + scalar);
}

vec2 vec2::operator-(const float &scalar) {
  return vec2(x - scalar, y - scalar);
}

vec2 vec2::operator+(const vec2 &other) {
  return vec2(x + other.x, y + other.y);
}

vec2 vec2::operator-(const vec2 &other) {
  return vec2(x - other.x, y - other.y);
}

vec2 vec2::operator*(const float &scalar) {
  return vec2(x * scalar, y * scalar);
}

vec2 vec2::operator/(const float &scalar) {
  return vec2(x / scalar, y / scalar);
}

vec2 vec2::operator*(const vec2 &other) {
  return vec2(x * other.x, y * other.y);
}

vec2 vec2::operator/(const vec2 &other) {
  return vec2(x / other.x, y / other.y);
}

float vec2::distance() { return sqrt(x * x + y * y); }

float vec2::dot(vec2 &other) { return x * other.x + y * other.y; }

vec2 vec2::negate() { return vec2(-x, -y); }
}
}
