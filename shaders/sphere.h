#pragma once

#include "intersection.h"
#include "material.h"
#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Sphere {
  const float3 center;
  const float radius;

  const Material material;

  // ray - the ray that intersection is checked upon
  // returns object with intersection details (didHit=false if didn't intersect)
  // NOTE: the field ray.direction is assumed to be normalized.
  Intersection intersect(const thread Ray &ray,
                         float minTime = 0) constant const {
    const float3 originToCenter = center - ray.origin;
    const float timeIntersectCenter = dot(originToCenter, ray.direction);
    if (timeIntersectCenter < minTime)
      return Intersection{.didHit = false};

    const float centerToRayLengthSquared =
        dot(originToCenter, originToCenter) -
        timeIntersectCenter * timeIntersectCenter;
    const float radiusSquared = radius * radius;

    const float discriminant = radiusSquared - centerToRayLengthSquared;

    if (discriminant < 0)
      return Intersection{.didHit = false};
    if (discriminant == 0) {
      Intersection intersection =
          Intersection{.didHit = true,
                       .time = timeIntersectCenter,
                       .point = ray.at(timeIntersectCenter),
                       .material = material};

      // P is on the sphere, so |P-C|=R, so dividing by R makes it normalized
      float3 normal = (intersection.point - center) / radius;

      intersection.setFaceNormal(ray, normal);

      return intersection;
    }

    const float intersectTimeVariation = discriminant * rsqrt(discriminant);

    float t1 = timeIntersectCenter - intersectTimeVariation;
    t1 = t1 < 0 ? INFINITY : t1;
    float t2 = timeIntersectCenter + intersectTimeVariation;
    t2 = t2 < 0 ? INFINITY : t2;

    const float time = min(t1, t2); // time is the smallest of the two times

    Intersection intersection = Intersection{.didHit = true,
                                             .time = time,
                                             .point = ray.at(time),
                                             .material = material};

    // P is on the sphere, so |P-C|=R, so dividing by R makes it normalized
    float3 normal = (intersection.point - center) / radius;

    intersection.setFaceNormal(ray, normal);

    return intersection;
  }
};
