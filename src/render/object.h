// CPU SIDE DEFINITION OF THE OBJECT STRUCTS
// USED WHEN TRANSFERING OBJECT LIST INTO THE GPU SIDE

#pragma once

#include <simd/simd.h>

struct Material {
  simd::float3 color;
  float emissionStrength;
  simd::float3 emissionColor;
};

struct Sphere {
  simd::float3 center;
  float radius;
  Material material;
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
