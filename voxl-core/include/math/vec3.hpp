#ifndef VOXL_MATH_VEC3_H_
#define VOXL_MATH_VEC3_H_

namespace voxl {
namespace math {
class Vec3 {
public:
  Vec3();
  Vec3(float xCoord, float yCoord, float zCoord);

  float x, y, z;

  Vec3 operator+(const float &scalar);
  Vec3 operator-(const float &scalar);

  Vec3 operator+(const Vec3 &other);
  Vec3 operator-(const Vec3 &other);

  Vec3 operator*(const float &scalar);
  Vec3 operator/(const float &scalar);

  Vec3 operator*(const Vec3 &other);
  Vec3 operator/(const Vec3 &other);

  float Distance();
  float Dot(Vec3 &other);
  Vec3 Cross(Vec3 &other);
  Vec3 Negate();
  Vec3 Normalize();
  static Vec3 Normalize(Vec3 vec);
};
}
}

#endif
