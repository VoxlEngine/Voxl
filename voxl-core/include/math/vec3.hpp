#ifndef VOXL_MATH_VEC3_H_
#define VOXL_MATH_VEC3_H_

namespace voxl {
namespace math {
class vec3 {
public:
  vec3();
  vec3(float xCoord, float yCoord, float zCoord);

  float x, y, z;

  vec3 operator+(const float &scalar);
  vec3 operator-(const float &scalar);

  vec3 operator+(const vec3 &other);
  vec3 operator-(const vec3 &other);

  vec3 operator*(const float &scalar);
  vec3 operator/(const float &scalar);

  vec3 operator*(const vec3 &other);
  vec3 operator/(const vec3 &other);

  float distance();
  float dot(vec3 &other);
  vec3 cross(vec3 &other);
  vec3 negate();
  vec3 normalize();
  static vec3 normalize(vec3 vec);
};
}
}

#endif
