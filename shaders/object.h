#pragma once

#include "intersection.h"
#include "ray.h"
#include "sphere.h"

#include <metal_stdlib>
using namespace metal;

struct Object {
  enum class Type {
    Sphere,
  };

  Type type;
  union {
    Sphere sphere;
  };

  // ray - the ray that intersection is checked upon
  // intersection is set if one is found, with the details of the intersection
  // returns whether the ray intersected the sphere or not
  // NOTE: the field ray.direction is assumed to be normalized.
  bool intersect(const thread Ray &ray,
                 thread Intersection &intersection) const {
    switch (type) {
    case Type::Sphere:
      return sphere.intersect(ray, intersection);
    }
  }
};
