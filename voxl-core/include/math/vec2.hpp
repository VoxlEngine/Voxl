#ifndef VOXL_MATH_VEC2_H_
#define VOXL_MATH_VEC2_H_

namespace voxl {
namespace math {
class vec2 {
public:
  vec2();
  vec2(float xCoord, float yCoord);

  float x, y;

  vec2 operator+(const float &scalar);
  vec2 operator-(const float &scalar);

  vec2 operator+(const vec2 &other);
  vec2 operator-(const vec2 &other);

  vec2 operator*(const float &scalar);
  vec2 operator/(const float &scalar);

  vec2 operator*(const vec2 &other);
  vec2 operator/(const vec2 &other);

  float distance();
  float dot(vec2 &other);
  vec2 negate();
};
}
}

#endif
