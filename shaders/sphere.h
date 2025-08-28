#pragma once

#include "intersection.h"
#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Sphere {
  float3 center;
  float radius;
  half3 color;

  // ray - the ray that intersection is checked upon
  // intersection is set if one is found, with the details of the intersection
  // returns whether the ray intersected the sphere or not
  // NOTE: the field ray.direction is assumed to be normalized.
  bool intersect(const thread Ray &ray,
                 thread Intersection &intersection) const {
    const float3 originToCenter = center - ray.origin;
    const float timeIntersectCenter = dot(originToCenter, ray.direction);
    if (timeIntersectCenter < 0)
      return false;

    const float centerToRayLengthSquared =
        dot(originToCenter, originToCenter) -
        timeIntersectCenter * timeIntersectCenter;
    const float radiusSquared = radius * radius;

    const float discriminant = radiusSquared - centerToRayLengthSquared;

    if (discriminant < 0)
      return false;
    if (discriminant == 0) {
      intersection = Intersection{.time = timeIntersectCenter,
                                  .point = ray.at(timeIntersectCenter)};

      // P is on the sphere, so |P-C|=R, so dividing by R makes it normalized
      float3 normal = (intersection.point - center) / radius;

      intersection.setFaceNormal(ray, normal);

      return true;
    }

    const float intersectTimeVariation = discriminant * rsqrt(discriminant);

    float t1 = timeIntersectCenter - intersectTimeVariation;
    t1 = t1 < 0 ? INFINITY : t1;
    float t2 = timeIntersectCenter + intersectTimeVariation;
    t2 = t2 < 0 ? INFINITY : t2;

    const float time = min(t1, t2); // time is the smallest of the two times

    intersection = Intersection{.time = time, .point = ray.at(time)};

    // P is on the sphere, so |P-C|=R, so dividing by R makes it normalized
    float3 normal = (intersection.point - center) / radius;

    intersection.setFaceNormal(ray, normal);

    return true;
  }
};
