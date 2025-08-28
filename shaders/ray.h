#pragma once

#include <metal_stdlib>
using namespace metal;

struct Ray {
  float3 origin;
  float3 direction;
  
  half3 skyboxColor() const {
    const float scaledY = (direction.y + 1.0f) * 0.5f;
    constexpr half3 bottomSkybox = half3(1.0f, 1.0f, 1.0f);
    constexpr half3 topSkybox = half3(0.5f, 0.7f, 1.0f);
    return (1 - scaledY) * bottomSkybox + scaledY * topSkybox;
  }
  
  float3 at(float t) const {
    return origin + direction * t;
  }
};
