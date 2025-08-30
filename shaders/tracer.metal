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
constant float focalLength = 4.f;

half3 trace(const thread Ray &initialRay, const thread Scene &scene, thread rngPCG32 &rng) {
  Ray ray = initialRay;
  float3 color = 1.0;
  float3 emittedLight = 0.0;
  
  for (size_t i = 0; i < maxBounceCount; ++i) {
    Intersection intersection = scene.intersect(ray);
    if (intersection.didHit) {
      float3 direction = randomUnitVector(rng);
      if (dot(direction, intersection.normal) < 0.0) direction = -direction;
      ray.origin = intersection.point;
      ray.direction = direction;
      
      float3 emission = intersection.material.emissionStrength * intersection.material.emissionColor;
      emittedLight += emission * color;
      
      color *= intersection.material.color;
      
      continue;
    }
    
    break;
    
    const float scaledY = (ray.direction.y + 1.0f) * 0.5f;
    constexpr float3 bottomSkybox = 1.0f;
    constexpr float3 topSkybox = float3(0.5f, 0.7f, 1.0f);
    emittedLight = color * ((1 - scaledY) * bottomSkybox + scaledY * topSkybox);
    
    break;
  }
  
  return half3(emittedLight);
}

struct Camera {
  float3 right;
  float3 up;
  float3 forward;
  float3 pos;
  
  Ray getRay(uint2 gid, float width, float height, float2 jitter = float2(0, 0)) constant const {
    const float aspect = width / height;

    float2 screen = (float2(gid) + jitter + 0.5) / float2(width, height) * 2.0 - 1.0;
    screen.x *= aspect;
    
    const float3 rayDir = normalize(screen.x * right + screen.y * up + forward * focalLength);
    Ray ray = Ray{pos, rayDir};
    
    return ray;
  }
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

  Scene scene = Scene{
    .objects=objects,
    .objectCount=*objectCount
  };
  
  half3 color = 0;
  
  for (size_t i = 0; i < samplesPerPixel; ++i) {
    float2 jitter = float2(randomFloatPCG32(rng), randomFloatPCG32(rng));
    Ray ray = camera->getRay(gid, width, height, jitter);
    
    color += trace(ray, scene, rng);
  }
  
  outTexture.write(half4(color / samplesPerPixel, 1.0), gid);

  // outTexture.write(half4(half3(rayDir), 1.0), gid);
}
