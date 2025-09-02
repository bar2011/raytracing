// CPU SIDE DEFINITION OF THE OBJECT STRUCTS
// USED WHEN TRANSFERING OBJECT LIST INTO THE GPU SIDE

#pragma once

#include <simd/simd.h>
#include <vector>

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
};

struct Triangle {
  simd::float3 a;
  simd::float3 b;
  simd::float3 c;

  // If true, the triangle is one-sided (back-face culled):
  // rays can only hit the front face. If false, the triangle is double-sided
  // and rays can hit it from either side.
  bool oneSided;
};

// Object: describes an object primitive
struct Object {
  enum class Type { Sphere, Triangle };

  Type type;
  union {
    Sphere sphere;
    Triangle triangle;
  };
};

// Mesh: passed to the actual shader in a mesh array and contains information
// about groups of objects
struct Mesh {
  // Index of the first object in the objects array
  uint32_t objectFirst;
  // Count of mesh objects
  uint32_t objectCount;
  // Material to apply to all objects of the mesh
  Material material;
};

// Model: a CPU-only struct which contains object + material information
struct Model {
  std::vector<Object> objects;
  Material material;
};
