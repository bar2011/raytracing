#pragma once

#include "material.h"
#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Intersection {
  bool didHit;
  float time;
  float3 point;
  float3 normal;
  bool frontFace;
  Material material;

  // NOTE: the parameter `outwardNormal` is assumed to be normalized.
  void setFaceNormal(const thread Ray &ray, float3 outwardNormal) {
    frontFace = dot(ray.direction, outwardNormal) < 0.0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};
