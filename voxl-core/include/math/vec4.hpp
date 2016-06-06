#ifndef VOXL_MATH_VEC4_H_
#define VOXL_MATH_VEC4_H_

namespace voxl {
namespace math {
class vec4 {
public:
  vec4();
  vec4(float xCoord, float yCoord, float zCoord, float wCoord);

  float x, y, z, w;

  vec4 operator+(const float &scalar);
  vec4 operator-(const float &scalar);

  vec4 operator+(const vec4 &other);
  vec4 operator-(const vec4 &other);

  vec4 operator*(const float &scalar);
  vec4 operator/(const float &scalar);

  vec4 operator*(const vec4 &other);
  vec4 operator/(const vec4 &other);

  float distance();
  float dot(vec4 &other);
  vec4 negate();
};
}
}

#endif
