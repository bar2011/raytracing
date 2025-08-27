#include <metal_stdlib>
using namespace metal;

struct VertexOutput {
  float4 position [[position]];
  float2 uv;
};

vertex VertexOutput vertexTexture(uint vertexID [[vertex_id]]) {
  constexpr float2 positions[6] {
    {-1.0f, -1.0f}, {+1.0f, -1.0f}, {+1.0f, +1.0f},
    {+1.0f, +1.0f}, {-1.0f, +1.0f}, {-1.0f, -1.0f},
  };

  constexpr float2 uvs[6] {
    {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f},
    {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
  };
  
  VertexOutput out = {
    .position = float4(positions[vertexID], 0.0f, 1.0f),
    .uv = uvs[vertexID]
  };


  return out;
}

fragment half4 fragmentTexture(VertexOutput in [[stage_in]],
                               texture2d<half, access::sample> texture [[texture(0)]]) {
  constexpr sampler textureSampler( address::repeat, filter::linear );
  return texture.sample(textureSampler, in.uv);
}
