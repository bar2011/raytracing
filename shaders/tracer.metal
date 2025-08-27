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

  float width = float(outTexture.get_width());
  float height = float(outTexture.get_height());

  if (gid.x >= width || gid.y >= height) return;

  float2 ndc = float2((0.5f + float(gid.x)) / width, (0.5f + float(gid.y)) / height);
  float2 screen = float2(2.0 * ndc.x - 1.0, 2.0 * ndc.y - 1.0);
  screen.x *= width / height;
  
  float3 rayDir = normalize(screen.x * camera->right + screen.y * camera->up + camera->forward);
  Ray ray = Ray{camera->pos, rayDir};

  outTexture.write(half4(half3(rayDir), 1.0), gid);
}
