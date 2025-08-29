#pragma once

#include <metal_stdlib>
using namespace metal;

struct Ray {
  float3 origin;
  float3 direction;

  float3 at(float t) const { return origin + direction * t; }
};
