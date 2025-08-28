#include <metal_stdlib>
using namespace metal;

struct Ray {
  float3 origin;
  float3 direction;
};




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
  const float focalLength = 1.5f;

  const float2 ndc = float2((0.5f + float(gid.x)) / width, (0.5f + float(gid.y)) / height);
  float2 screen = 2.0 * ndc - 1.0;
  screen.x *= aspect;
  
  const float3 rayDir = normalize(screen.x * camera->right + screen.y * camera->up + camera->forward * focalLength);
  Ray ray = Ray{camera->pos, rayDir};

  outTexture.write(half4(half3(rayDir), 1.0), gid);
}
