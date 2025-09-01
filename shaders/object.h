#pragma once

#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"

#include <metal_stdlib>
using namespace metal;

struct Object {
  enum class Type {
    Sphere, Triangle,
  };

  Type type;
  union {
    Sphere sphere;
    Triangle triangle;
  };

  // ray - the ray that intersection is checked upon
  // returns object with intersection details (didHit=false if didn't intersect)
  // NOTE: the field ray.direction is assumed to be normalized.
  Intersection intersect(const thread Ray &ray,
                         float minTime = 0) constant const {
    switch (type) {
    case Type::Sphere:
      return sphere.intersect(ray, minTime);
    case Type::Triangle:
      return triangle.intersect(ray, minTime);
    default:
      return Intersection{.didHit = false};
    }
  }
};
