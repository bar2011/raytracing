// CPU SIDE DEFINITION OF THE OBJECT STRUCTS
// USED WHEN TRANSFERING OBJECT LIST INTO THE GPU SIDE

#pragma once

#include <simd/simd.h>

struct Sphere {
  simd::float3 center;
  float radius;
};

struct Object {
  enum class Type {
    Sphere,
  };

  Type type;
  union {
    Sphere sphere;
  };
};
