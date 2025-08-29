#pragma once

#include "object.h"

#include <metal_stdlib>
using namespace metal;

struct Scene {
  constant const Object *objects;
  uint32_t objectCount;

  Intersection intersect(const thread Ray &ray) const {
    Intersection closestIntersection = {.didHit = false, .time=INFINITY};
    for (size_t i = 0; i < objectCount; ++i) {
      thread const Object object = objects[i];
      Intersection intersection = object.intersect(ray);
      if (intersection.didHit && intersection.time < closestIntersection.time)
        closestIntersection = intersection;
    }

    return closestIntersection;
  }
};
