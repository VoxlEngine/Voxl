#include <iostream>
#include <math.h>

#include "math/mat4.hpp"

namespace voxl {
namespace math {
mat4::mat4(float diagonal) {
  for (int i = 0; i < 16; i++) {
    elements[i] = 0;
  }

  set(0, 0, diagonal);
  set(1, 1, diagonal);
  set(2, 2, diagonal);
  set(3, 3, diagonal);
}

mat4 mat4::operator*(mat4 &other) {
  mat4 ret(0.0f);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float sum = 0;

      for (int k = 0; k < 4; k++) {
        sum += get(j, k) * other.get(k, i);
      }
      ret.set(j, i, sum);
    }
  }
  return ret;
}

vec4 mat4::operator*(vec4 &other) {
  vec4 ret(0.0f, 0.0f, 0.0f, 0.0f);

  for (int i = 0; i < 4; i++) {
    float sum = 0;
    sum += get(i, 0) * other.x;
    sum += get(i, 1) * other.y;
    sum += get(i, 2) * other.z;
    sum += get(i, 3) * other.w;

    switch (i) {
    case 0:
      ret.x = sum;

    case 1:
      ret.y = sum;

    case 2:
      ret.z = sum;
    }
    return ret;
  }
}

void mat4::operator*=(mat4 &other) { *this = *this * other; }

mat4 mat4::translate(vec3 delta) {
  mat4 ret(1.0f);

  ret.set(0, 3, delta.x);
  ret.set(1, 3, delta.y);
  ret.set(2, 3, delta.z);

  return ret;
}

mat4 mat4::rotateX(float radians) {
  mat4 ret(1.0f);

  ret.set(1, 1, cos(radians));
  ret.set(1, 2, -sin(radians));
  ret.set(2, 1, sin(radians));
  ret.set(2, 2, cos(radians));

  return ret;
}

mat4 mat4::rotateY(float radians) {
  mat4 ret(1.0f);

  ret.set(0, 0, cos(radians));
  ret.set(0, 2, sin(radians));
  ret.set(2, 0, -sin(radians));
  ret.set(2, 2, cos(radians));

  return ret;
}

mat4 mat4::rotateZ(float radians) {
  mat4 ret(1.0f);

  ret.set(0, 0, cos(radians));
  ret.set(0, 1, -sin(radians));
  ret.set(1, 0, sin(radians));
  ret.set(1, 1, cos(radians));

  return ret;
}

mat4 mat4::lookAt(vec3 right, vec3 up, vec3 dir, vec3 pos) {
  mat4 mat1(1.0f);
  mat4 mat2(1.0f);

  mat1.set(0, 0, right.x);
  mat1.set(0, 1, right.y);
  mat1.set(0, 2, right.z);
  mat1.set(1, 0, up.x);
  mat1.set(1, 1, up.y);
  mat1.set(1, 2, up.z);
  mat1.set(2, 0, dir.x);
  mat1.set(2, 1, dir.y);
  mat1.set(2, 2, dir.z);

  mat2.set(0, 3, -pos.x);
  mat2.set(1, 3, -pos.y);
  mat2.set(2, 3, -pos.z);

  return mat1 * mat2;
}

mat4 mat4::ortho(float left, float right, float bottom, float top, float near,
                 float far) {
  mat4 ret(1.0f);

  ret.set(0, 0, 2 / (right - left));
  ret.set(1, 1, 2 / (top - bottom));
  ret.set(1, 1, -2 / (far - near));

  ret.set(0, 3, -((right + left) / (right - left)));
  ret.set(1, 3, -((top + bottom) / (top - bottom)));
  ret.set(2, 3, -((far + near) / (far - near)));

  return ret;
}

mat4 mat4::persp(float fov, float aspectRatio, float near, float far) {
  mat4 ret(0.0f);

  float range = near - far;
  float tanHalfFov = tan(fov / 2);

  ret.set(0, 0, 1 / (tanHalfFov * aspectRatio));
  ret.set(1, 1, 1 / tanHalfFov);
  ret.set(2, 2, -far / range);
  ret.set(2, 3, (far * near) / range);
  ret.set(3, 2, 1);

  return ret;
}

void mat4::print() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << "| " << get(i, j) << " ";
    }
    std::cout << std::endl;
  }
}
}
}
