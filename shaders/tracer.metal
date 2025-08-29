#include "ray.h"
#include "intersection.h"
#include "sphere.h"
#include "object.h"

#include <metal_stdlib>
using namespace metal;

struct Camera {
  float3 right;
  float3 up;
  float3 forward;
  float3 pos;
};

kernel void tracer(
  texture2d<half, access::write> outTexture [[texture(0)]],
  constant unsigned long* shaderIter [[buffer(0)]],
  constant Camera* camera [[buffer(1)]],
  uint2 gid [[thread_position_in_grid]]) {

  const float width = float(outTexture.get_width());
  const float height = float(outTexture.get_height());

  if (gid.x >= width || gid.y >= height) return;
  
  const float aspect = width / height;
  const float focalLength = 4.0f;

  float2 screen = (float2(gid) + 0.5) / float2(width, height) * 2.0 - 1.0;
  screen.x *= aspect;
  
  const float3 rayDir = normalize(screen.x * camera->right + screen.y * camera->up + camera->forward * focalLength);
  Ray ray = Ray{camera->pos, rayDir};
  
  constexpr Object objects[] = {
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(0.0, 0.0, -5.0),
              .radius = 1.0,
              .color = half3(1.0, 0.0, 0.0) // red
          }
      },
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(2.0, 0.0, -6.0),
              .radius = 0.5,
              .color = half3(0.0, 1.0, 0.0) // green
          }
      },
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(-1.5, 1.0, -4.0),
              .radius = 0.75,
              .color = half3(0.0, 0.0, 1.0) // blue
          }
      },
      Object{
          .type = Object::Type::Sphere,
          .sphere = Sphere{
              .center = float3(3.0, 1.0, 4.0),
              .radius = 2.0,
              .color = half3(1.0, 1.0, 0.0) // yellow
      }
    },
  };

  Intersection closestIntersection = { .time = INFINITY };
  for (size_t i = 0; i < 3; ++i) {
    Intersection intersection;
    if (objects[i].intersect(ray, intersection) && intersection.time < closestIntersection.time)
      closestIntersection = intersection;
  }
  if (closestIntersection.time != INFINITY)
    outTexture.write(half4(half3((closestIntersection.normal + 1.0) / 2.0), 1.0), gid);
  else
    outTexture.write(half4(ray.skyboxColor(), 1.0), gid);

  // outTexture.write(half4(half3(rayDir), 1.0), gid);
}
