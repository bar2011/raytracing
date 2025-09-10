#pragma once

#include "random.h"
#include "ray.h"

#include <metal_stdlib>
using namespace metal;

struct Intersection;

struct Material {
  enum class Type { Metal, Matte };

  float3 color;
  float emissionStrength;
  float3 emissionColor;
  Type type;
  float fuzz;
  
  // Function which applies scatter/specular on the given ray, and puts the result in the same ray reference
  // Returns whether the interaction was successful (i.e. whether the ray was absorbed or reflected)
  static bool interact(const thread Intersection &intersection, thread Ray &ray, thread rngPCG32 &rng);

  // Using intersection, updates given ray to be a matching randomly generated
  // (via rng) scatter ray
  static bool scatter(const thread Intersection &intersection, thread Ray &ray,
                      thread rngPCG32 &rng);

  // Using intersection, updates given ray to be a reflected ray of itself
  static bool specular(const thread Intersection &intersection, thread Ray &ray,
                       thread rngPCG32 &rng);
};
