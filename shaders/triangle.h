#pragma once

#include "intersection.h"
#include "material.h"
#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Triangle {
  const float3 a;
  const float3 b;
  const float3 c;

  // If true, the triangle is one-sided (back-face culled):
  // rays can only hit the front face. If false, the triangle is double-sided
  // and rays can hit it from either side.
  bool oneSided;

  // ray - the ray that intersection is checked upon
  // returns object with intersection details (didHit=false if didn't intersect)
  // NOTE: the field ray.direction is assumed to be normalized.
  Intersection intersect(const thread Ray &ray,
                         float minTime = 0) constant const {
    // Vectors of all edges including A
    const float3 edge1 = b - a;
    const float3 edge2 = c - a;
    // Unnormalized
    const float3 normal = cross(edge1, edge2);

    float determinant = -dot(ray.direction, normal);

    if (abs(determinant) < 1e-5)
      return Intersection{.didHit = false};

    if (oneSided && determinant < 0.f)
      return Intersection{.didHit = false};

    float inverseDeterminant = 1.f / determinant;

    float3 tvec = ray.origin - a;

    float3 tvecCrossDir = cross(tvec, ray.direction);

    float u = dot(edge2, tvecCrossDir) * inverseDeterminant;
    if (u < 0 || u > 1)
      return Intersection{.didHit = false};

    float v = -dot(edge1, tvecCrossDir) * inverseDeterminant;
    if (v < 0 || u + v > 1)
      return Intersection{.didHit = false};

    float time = dot(tvec, normal) * inverseDeterminant;

    if (time < minTime)
      return Intersection{.didHit = false};

    Intersection intersection =
        Intersection{.didHit = true, .time = time, .point = ray.at(time)};

    intersection.setFaceNormal(ray, normalize(normal));

    return intersection;
  }
};
