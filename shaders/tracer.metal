#include "intersection.h"
#include "object.h"
#include "random.h"
#include "ray.h"
#include "scene.h"
#include "sphere.h"
#include "triangle.h"

#include <metal_stdlib>
using namespace metal;

constant size_t maxBounceCount = 6;
constant size_t samplesPerPixel = 10;
constant float focalLength = 4.f;
// 1 is regular strength, 0 is black / nonexistant
constant float environmentColorStrength = 1.f;
constant float sunStrength = 4.f;

float3 getEnvironmentColor(const thread Ray &ray) {
  // Define environment colors
  constexpr float3 groundColor = float3(0.3f, 0.3f, 0.3f);
  constexpr float3 skyColor = float3(0.05f, 0.2f, 0.7f);

  float3 baseColor = mix(groundColor, skyColor, ray.direction.y / 2.f + 0.5f) *
                     environmentColorStrength;

  // Sun contribution
  const float3 sunDir = normalize(float3(0.5f, 0.3f, 0.4f));
  constexpr float3 sunColor = float3(1.0f, 1.0f, 0.7f);
  float sunIntensity =
      pow(max(dot(ray.direction, sunDir), 0.0f), 500.f) * sunStrength;
  baseColor += sunColor * sunIntensity;

  return baseColor;
}

float3 trace(const thread Ray &initialRay, const thread Scene &scene,
            thread rngPCG32 &rng) {
  Ray ray = initialRay;
  float3 pixelColor = 1.0;
  float3 throughputColor = 0.0;

  for (size_t i = 0; i < maxBounceCount; ++i) {
    Intersection intersection = scene.intersect(ray);
    if (intersection.didHit) {
      float3 rayDirIn = -ray.direction;
      intersection.material.interact(intersection, ray, rng);

      float3 emission = intersection.material.emissionStrength *
                        intersection.material.emissionColor;
      throughputColor += emission * pixelColor;

      // Compute bounce weight as cos(alpha) * 2
      // According to the lambertian diffuse cosine weighing
      float bounceWeight = dot(intersection.normal, rayDirIn) * 2;
      pixelColor *= intersection.material.color * bounceWeight;
      
      if (!all(isfinite(throughputColor))) break;
      
      continue;
    }

    throughputColor += pixelColor * getEnvironmentColor(ray);
    break;
  }
  
  if (!all(isfinite(throughputColor))) return float3(0, 0, 0);

  return throughputColor;
}

struct Camera {
  float3 right;
  float3 up;
  float3 forward;
  float3 pos;

  Ray getRay(uint2 gid, float width, float height,
             float2 jitter = float2(0, 0)) constant const {
    const float aspect = width / height;

    float2 screen =
        (float2(gid) + jitter + 0.5) / float2(width, height) * 2.0 - 1.0;
    screen.x *= aspect;

    const float3 rayDir =
        normalize(screen.x * right + screen.y * up + forward * focalLength);
    Ray ray = Ray{pos, rayDir};

    return ray;
  }
};

kernel void tracer(texture2d<float, access::read_write> outTexture
                   [[texture(0)]],
                   constant uint32_t *shaderIter [[buffer(0)]],
                   constant bool *retainTexture [[buffer(1)]],
                   constant Camera *camera [[buffer(2)]],
                   constant Object *objects [[buffer(3)]],
                   constant uint32_t *objectCount [[buffer(4)]],
                   uint2 gid [[thread_position_in_grid]]) {
  const float width = float(outTexture.get_width());
  const float height = float(outTexture.get_height());

  if (gid.x >= width || gid.y >= height)
    return;

  const uint64_t seed =
      ((uint64_t)gid.y * outTexture.get_width() + gid.x) + 1 +
      *shaderIter * outTexture.get_width() * outTexture.get_height();
  rngPCG32 rng = rngPCG32{.state = seed, .inc = (seed << 1) | 1};
  // Advance the RNG to ensure randomness
  randomIntPCG32(rng);

  Scene scene = Scene{.objects = objects, .objectCount = *objectCount};

  float3 color = 0;

  for (size_t i = 0; i < samplesPerPixel; ++i) {
    float2 jitter = float2(randomFloatPCG32(rng), randomFloatPCG32(rng));
    Ray ray = camera->getRay(gid, width, height, jitter);

    color += trace(ray, scene, rng);
  }

  // If retainTexture is true, append current color to the previous color in the
  // texture In the fragmentTexture takes the average of the texture throughout
  // the iterations
  float4 finalColor = float4(color / samplesPerPixel, 1.0);
  if (*retainTexture)
    outTexture.write(finalColor + outTexture.read(gid), gid);
  else
    outTexture.write(finalColor, gid);

  // outTexture.write(half4(half3(rayDir), 1.0), gid);
}
