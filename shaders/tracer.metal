#include "ray.h"
#include "intersection.h"
#include "sphere.h"
#include "object.h"
#include "scene.h"

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
  constant Object* objects [[buffer(2)]],
  constant uint32_t* objectCount [[buffer(3)]],
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
  
  Scene scene = Scene{
    .objects=objects,
    .objectCount=*objectCount
  };

  Intersection intersection = scene.intersect(ray);
  if (intersection.didHit)
    outTexture.write(half4(half3((intersection.normal + 1.0) / 2.0), 1.0), gid);
  else
    outTexture.write(half4(ray.skyboxColor(), 1.0), gid);

  // outTexture.write(half4(half3(rayDir), 1.0), gid);
}
