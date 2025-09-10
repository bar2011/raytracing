#include "material.h"
#include "intersection.h"
#include "ray.h"
#include "random.h"

bool Material::interact(const thread Intersection &intersection, thread Ray &ray, thread rngPCG32 &rng) {
  switch (intersection.material.type) {
    case Type::Matte:
      return scatter(intersection, ray, rng);
    case Type::Metal:
      return specular(intersection, ray, rng);
  }
}

bool Material::scatter(const thread Intersection &intersection, thread Ray &ray, thread rngPCG32 &rng) {
  ray.origin = intersection.point;
  ray.direction = randomUnitVector(rng);
  if (dot(ray.direction, intersection.normal) < 0.0)
    ray.direction *= -1;
  return true;
}

bool Material::specular(const thread Intersection &intersection, thread Ray &ray, thread rngPCG32 &rng) {
  ray.origin = intersection.point;
  ray.direction =
      ray.direction -
      2 * dot(ray.direction, intersection.normal) * intersection.normal;
  if (intersection.material.fuzz > 0) {
    ray.direction = normalize(ray.direction + intersection.material.fuzz * randomUnitVector(rng));
    if (dot(ray.direction, intersection.normal) <= 0.0) return false;
  }
  return true;
}
