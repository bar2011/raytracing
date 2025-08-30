#pragma once

#include <metal_stdlib>
using namespace metal;

struct Material {
  float3 color;
  float emissionStrength;
  float3 emissionColor;
};
