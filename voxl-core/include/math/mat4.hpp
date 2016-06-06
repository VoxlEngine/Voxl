#ifndef VOXL_MATH_MAT4_H_
#define VOXL_MATH_MAT4_H_

#include "types.hpp"

#include "math/vec3.hpp"
#include "math/vec4.hpp"

namespace voxl {
namespace math {
class mat4 {
public:
  mat4(float diagonal);

  inline float get(uint i, uint j) { return elements[i + j * 4]; };
  inline float *getElements() { return elements; };
  inline void set(uint i, uint j, float val) { elements[i + j * 4] = val; };

  mat4 operator*(mat4 &other);
  vec4 operator*(vec4 &other);
  void operator*=(mat4 &other);

  static mat4 translate(vec3 delta);

  static mat4 rotateX(float radians);
  static mat4 rotateY(float radians);
  static mat4 rotateZ(float radians);

  static mat4 lookAt(vec3 right, vec3 up, vec3 dir, vec3 pos);

  static mat4 ortho(float left, float right, float bottom, float top,
                    float near, float far);
  static mat4 persp(float fov, float aspectRatio, float near, float far);

  void print();

private:
  float elements[16];
};
}
}

#endif
