#pragma once

#include <metal_stdlib>
using namespace metal;

// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)
struct rngPCG32 {
  uint64_t state;
  uint64_t inc;
};

inline uint32_t randomIntPCG32(thread rngPCG32 &rng) {
    const uint64_t oldstate = rng.state;
    // Advance internal state
    rng.state = oldstate * 6364136223846793005ULL + (rng.inc | 1);
    // Calculate output function (XSH RR), uses old state for max ILP
    const uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    const uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

inline float randomFloatPCG32(thread rngPCG32 &rng) {
  const uint32_t val = randomIntPCG32(rng);
  return val / 4294967295.0f; // 2^32-1
}

// Credit: https://stackoverflow.com/a/6178290
inline float randomGaussianPCG32(thread rngPCG32 &rng) {
  const float theta = 2 * M_PI_F * randomFloatPCG32(rng);
  const float rho = sqrt(-2.0f * log(randomFloatPCG32(rng)));
  return rho * cos(theta);
}

inline float3 randomUnitVector(thread rngPCG32 &rng) {
  const float x = randomGaussianPCG32(rng);
  const float y = randomGaussianPCG32(rng);
  const float z = randomGaussianPCG32(rng);
  
  return normalize(float3{x, y, z});
}
