#include <iostream>
#include <math.h>

#include "math/mat4.hpp"

namespace voxl {
namespace math {
Mat4::Mat4(float diagonal) {
  for (int i = 0; i < 16; i++) {
    elements[i] = 0;
  }

  Set(0, 0, diagonal);
  Set(1, 1, diagonal);
  Set(2, 2, diagonal);
  Set(3, 3, diagonal);
}

Mat4 Mat4::operator*(Mat4 &other) {
  Mat4 ret(0.0f);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float sum = 0;
      for (int k = 0; k < 4; k++) {
        sum += Get(j, k) * other.Get(k, i);
      }
      ret.Set(j, i, sum);
    }
  }
  return ret;
}

Vec4 Mat4::operator*(Vec4 &other) {
  Vec4 ret(0.0f, 0.0f, 0.0f, 0.0f);
  for (int i = 0; i < 4; i++) {
    float sum = 0;
    sum += Get(i, 0) * other.x;
    sum += Get(i, 1) * other.y;
    sum += Get(i, 2) * other.z;
    sum += Get(i, 3) * other.w;
    switch (i) {
    case 0:
      ret.x = sum;
    case 1:
      ret.y = sum;
    case 2:
      ret.z = sum;
    case 3:
      ret.w = sum;
    }
  }
  return ret;
}

void Mat4::operator*=(Mat4 &other) { *this = *this * other; }

Mat4 Mat4::Translate(Vec3 delta) {
  Mat4 ret(1.0f);

  ret.Set(0, 3, delta.x);
  ret.Set(1, 3, delta.y);
  ret.Set(2, 3, delta.z);

  return ret;
}

Mat4 Mat4::RotateX(float radians) {
  Mat4 ret(1.0f);

  ret.Set(1, 1, cos(radians));
  ret.Set(1, 2, -sin(radians));
  ret.Set(2, 1, sin(radians));
  ret.Set(2, 2, cos(radians));

  return ret;
}

Mat4 Mat4::RotateY(float radians) {
  Mat4 ret(1.0f);

  ret.Set(0, 0, cos(radians));
  ret.Set(0, 2, sin(radians));
  ret.Set(2, 0, -sin(radians));
  ret.Set(2, 2, cos(radians));

  return ret;
}

Mat4 Mat4::RotateZ(float radians) {
  Mat4 ret(1.0f);

  ret.Set(0, 0, cos(radians));
  ret.Set(0, 1, -sin(radians));
  ret.Set(1, 0, sin(radians));
  ret.Set(1, 1, cos(radians));

  return ret;
}

Mat4 Mat4::LookAt(Vec3 right, Vec3 up, Vec3 dir, Vec3 pos) {
  Mat4 mat1(1.0f);
  Mat4 mat2(1.0f);

  mat1.Set(0, 0, right.x);
  mat1.Set(0, 1, right.y);
  mat1.Set(0, 2, right.z);
  mat1.Set(1, 0, up.x);
  mat1.Set(1, 1, up.y);
  mat1.Set(1, 2, up.z);
  mat1.Set(2, 0, dir.x);
  mat1.Set(2, 1, dir.y);
  mat1.Set(2, 2, dir.z);

  mat2.Set(0, 3, -pos.x);
  mat2.Set(1, 3, -pos.y);
  mat2.Set(2, 3, -pos.z);

  return mat1 * mat2;
}

Mat4 Mat4::Ortho(float left, float right, float bottom, float top, float near,
                 float far) {
  Mat4 ret(1.0f);

  ret.Set(0, 0, 2 / (right - left));
  ret.Set(1, 1, 2 / (top - bottom));
  ret.Set(1, 1, -2 / (far - near));

  ret.Set(0, 3, -((right + left) / (right - left)));
  ret.Set(1, 3, -((top + bottom) / (top - bottom)));
  ret.Set(2, 3, -((far + near) / (far - near)));

  return ret;
}

Mat4 Mat4::Persp(float fov, float aspectRatio, float near, float far) {
  Mat4 ret(0.0f);

  float range = near - far;
  float tanHalfFov = tan(fov / 2);

  ret.Set(0, 0, 1 / (tanHalfFov * aspectRatio));
  ret.Set(1, 1, 1 / tanHalfFov);
  ret.Set(2, 2, -far / range);
  ret.Set(2, 3, (far * near) / range);
  ret.Set(3, 2, 1);

  return ret;
}

void Mat4::Print() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << "| " << Get(i, j) << " ";
    }
    std::cout << std::endl;
  }
}
}
}
