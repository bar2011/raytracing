#pragma once

#include "object.h"
#include "mesh.h"

#include <metal_stdlib>
using namespace metal;

struct Scene {
  constant const Object *objects;
  uint32_t objectCount;
  constant const Mesh *meshes;
  uint32_t meshCount;

  Intersection intersect(const thread Ray &ray) const {
    Intersection closestIntersection = {.didHit = false, .time = INFINITY};
    for (size_t i = 0; i < meshCount; ++i) {
      Intersection intersection = meshes[i].intersect(ray, objects, 1e-3f);
      if (intersection.didHit && intersection.time < closestIntersection.time)
        closestIntersection = intersection;
    }

    return closestIntersection;
  }
};
