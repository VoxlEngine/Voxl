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

float Vec2::Distance() { return sqrt(x * x + y * y); }

float Vec2::Dot(Vec2 &other) { return x * other.x + y * other.y; }

Vec2 Vec2::Negate() { return Vec2(-x, -y); }
}
}
