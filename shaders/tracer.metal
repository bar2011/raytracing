#include <metal_stdlib>
using namespace metal;

kernel void tracer(
    texture2d<half, access::write> outTexture [[texture(0)]],
    constant unsigned long* shaderIter [[buffer(0)]],
    uint2 gid [[thread_position_in_grid]]) {

    float width = float(outTexture.get_width());
    float height = float(outTexture.get_height());

    if (gid.x >= width || gid.y >= height) return;

    // Constants: Final zoom target point (center), initial screen boundaries, and zoom rate
    constexpr float finalX = -0.09;
    constexpr float finalY = 0.87;

    constexpr float initialLeft = -2.5;
    constexpr float initialRight = 2.5;
    constexpr float initialBottom = -2.5;
    constexpr float initialTop = 2.5;

    constexpr float rate = 240.0;

    // Assume *shaderIter is available as a pointer to the current iteration/frame count
    float decay = exp(-float(*shaderIter) / rate);

    // Compute current boundaries for this frame
    float left   = finalX + (initialLeft   - finalX) * decay;
    float right  = finalX + (initialRight  - finalX) * decay;
    float bottom = finalY + (initialBottom - finalY) * decay;
    float top    = finalY + (initialTop    - finalY) * decay;

    float2 c = float2(
        (float(gid.x) + 0.5) / width * (right - left) + left,
        (float(gid.y) + 0.5) / height * (top - bottom) + bottom);

    float2 z = float2(0, 0);
  
    const uint maxIter = 50;
    for (uint i = 0; i < maxIter; ++i) {
      // Check if the squared magnitude is bigger then 2^2
      float magSqrd = z.x * z.x + z.y * z.y;
      if (magSqrd >= 4) {
        outTexture.write(half4(1.0, 1.0, 1.0, 1.0), gid);
        return;
      }
      // Complex square plus initial value
      z = float2(z.x * z.x - z.y * z.y + c.x, 2 * z.x * z.y + c.y);
    }

    outTexture.write(half4(0.0, 0.0, 0.0, 1.0), gid);
}
