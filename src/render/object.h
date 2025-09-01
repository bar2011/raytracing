// CPU SIDE DEFINITION OF THE OBJECT STRUCTS
// USED WHEN TRANSFERING OBJECT LIST INTO THE GPU SIDE

#pragma once

#include <simd/simd.h>

struct Material {
  enum class Type { Metal, Matte };
  simd::float3 color;
  float emissionStrength;
  simd::float3 emissionColor;
  Type type;
};

struct Sphere {
  simd::float3 center;
  float radius;

  Material material;
};

struct Triangle {
  simd::float3 a;
  simd::float3 b;
  simd::float3 c;

  // If true, the triangle is one-sided (back-face culled):
  // rays can only hit the front face. If false, the triangle is double-sided
  // and rays can hit it from either side.
  bool oneSided;

  Material material;
};

struct Object {
  enum class Type { Sphere, Triangle };

  Type type;
  union {
    Sphere sphere;
    Triangle triangle;
  };
};
