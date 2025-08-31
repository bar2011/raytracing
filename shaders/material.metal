#include "material.h"
#include "intersection.h"
#include "ray.h"
#include "random.h"

void Material::interact(const thread Intersection &intersection, thread Ray &ray, thread rngPCG32 &rng) {
  switch (type) {
    case Type::Matte:
      return scatter(intersection, ray, rng);
    case Type::Metal:
      return specular(intersection, ray);
  }
}

void Material::scatter(const thread Intersection &intersection, thread Ray &ray, thread rngPCG32 &rng) {
  ray.origin = intersection.point;
  ray.direction = randomUnitVector(rng);
  if (dot(ray.direction, intersection.normal) < 0.0)
    ray.direction *= -1;
}

void Material::specular(const thread Intersection &intersection, thread Ray &ray) {
  ray.origin = intersection.point;
  ray.direction =
      ray.direction -
      2 * dot(ray.direction, intersection.normal) * intersection.normal;
}
