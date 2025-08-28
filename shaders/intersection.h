#pragma once

#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Intersection {
  float time;
  float3 point;
  float3 normal;
  bool frontFace;

  // NOTE: the parameter `outward_normal` is assumed to be normalized.
  void setFaceNormal(const thread Ray &ray, float3 outwardNormal) {
    frontFace = dot(ray.direction, outwardNormal) < 0.0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};
