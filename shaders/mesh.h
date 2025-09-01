#pragma once

#include "intersection.h"
#include "material.h"
#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Mesh {
  // Index of the first object in the objects array
  uint32_t objectFirst;
  // Count of mesh objects
  uint32_t objectCount;
  // Material to apply to all objects of the mesh
  Material material;
  
  Intersection intersect(const thread Ray &ray, constant const Object *objects, float minTime = 0) constant const {
    Intersection closestIntersection = {.didHit = false, .time = INFINITY};
    for (size_t i = objectFirst; i < objectFirst + objectCount; ++i) {
      Intersection intersection = objects[i].intersect(ray, minTime);
      if (intersection.didHit && intersection.time < closestIntersection.time)
        closestIntersection = intersection;
    }
    
    closestIntersection.material = material;

    return closestIntersection;
  }
};
