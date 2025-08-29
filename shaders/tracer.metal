#include "ray.h"
#include "intersection.h"
#include "sphere.h"
#include "object.h"
#include "scene.h"
#include "random.h"

#include <metal_stdlib>
using namespace metal;

constant size_t maxBounceCount = 8;
constant size_t samplesPerPixel = 10;

half3 trace(const thread Ray &initialRay, const thread Scene &scene, thread rngPCG32 &rng) {
  Ray ray = initialRay;
  half3 color = 1.0;
  
  for (size_t i = 0; i < maxBounceCount; ++i) {
    Intersection intersection = scene.intersect(ray);
    if (intersection.didHit) {
      float3 direction = randomUnitVector(rng);
      if (dot(direction, intersection.normal) < 0.0) direction = -direction;
      ray.origin = intersection.point;
      ray.direction = direction;
      color *= 0.3;
      continue;
    }
    
    break;
    const float scaledY = (ray.direction.y + 1.0f) * 0.5f;
    constexpr half3 bottomSkybox = 1.0f;
    constexpr half3 topSkybox = half3(0.5f, 0.7f, 1.0f);
    return color * ((1 - scaledY) * bottomSkybox + scaledY * topSkybox);
  }
  
  return color;
}

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
  
  const uint64_t seed = ((uint64_t)gid.y * outTexture.get_width() + gid.x) + 1;
  rngPCG32 rng = rngPCG32{ .state=seed, .inc=(seed << 1) | 1 };
  // Advance the RNG to ensure randomness
  randomIntPCG32(rng);

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
  
  half3 color = 0;
  
  for (size_t i = 0; i < samplesPerPixel; ++i)
    color += trace(ray, scene, rng);
  
  outTexture.write(half4(color / samplesPerPixel, 1.0), gid);

  // outTexture.write(half4(half3(rayDir), 1.0), gid);
}
