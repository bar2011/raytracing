#pragma once

#include "Metal/MTLComputePipeline.hpp"
#include "Metal/MTLTexture.hpp"
#include "object.h"
#include "utils/camera.h"
#include <MetalKit/MetalKit.hpp>
#include <cstdint>

class Renderer {
public:
  Renderer(MTL::Device *device);

  ~Renderer();

  void draw(MTK::View *view);

private:
  void buildShaders();
  void buildTexture();

  MTL::Device *m_device;
  MTL::CommandQueue *m_commandQueue;
  MTL::Library *m_defaultLibrary;

  MTL::ComputePipelineState *m_tracerPipeline{nullptr};
  MTL::RenderPipelineState *m_texturePipeline{nullptr};

  std::shared_ptr<Camera> m_camera{std::make_shared<Camera>(0.1f, 0.2f)};

  MTL::Texture *m_screenTexture{};
  // Holds the iteration num since m_retainTexture changed
  uint32_t m_iteration{};
  // If true, will keep adding results to texture and just show their average
  bool m_retainTexture{false};

  std::array<Object, 5> m_objects = {{
      {.type = Object::Type::Sphere,
       .sphere = {.center = {0.0f, 1.0f, 0.0f},
                  .radius =
                      1.0f}}, // Glowing sphere hovering at (0,1,0), radius 1
      {.type = Object::Type::Triangle,
       .triangle = {.a = {-5.0f, 0.0f, -5.0f},
                    .b = {5.0f, 0.0f, -5.0f},
                    .c = {-5.0f, 0.0f, 5.0f},
                    .oneSided =
                        true}}, // First half of ground plane (left triangle)
      {.type = Object::Type::Triangle,
       .triangle = {.a = {5.0f, 0.0f, -5.0f},
                    .b = {5.0f, 0.0f, 5.0f},
                    .c = {-5.0f, 0.0f, 5.0f},
                    .oneSided =
                        true}}, // Second half of ground plane (right triangle)
                                // → together they form a 10x10 metallic floor
      {.type = Object::Type::Triangle,
       .triangle = {.a = {-5.0f, 0.0f, -5.0f},
                    .b = {5.0f, 0.0f, -5.0f},
                    .c = {-5.0f, 5.0f, -10.0f},
                    .oneSided = false}}, // First half of backdrop wall (left
                                         // triangle) tilted slightly backward
      {.type = Object::Type::Triangle,
       .triangle = {.a = {5.0f, 0.0f, -5.0f},
                    .b = {5.0f, 5.0f, -10.0f},
                    .c = {-5.0f, 5.0f, -10.0f},
                    .oneSided = false}} // Second half of backdrop wall (right
                                        // triangle) → together they form a
                                        // vertical rectangle behind sphere
  }};

  std::array<Mesh, 2> m_meshes = {{
      {.objectFirst = 0,
       .objectCount = 1,
       .material =
           {.color = {1.0f, 0.2f, 0.2f},
            .emissionStrength = 5.0f,
            .emissionColor = {1.0f, 0.3f, 0.3f},
            .type = Material::Type::Metal}}, // Mesh 0 → glowing red matte
                                             // sphere (emissive light source)
      {.objectFirst = 1,
       .objectCount = 4,
       .material =
           {.color = {0.8f, 0.8f, 0.9f},
            .emissionStrength = 0.0f,
            .emissionColor = {0.0f, 0.0f, 0.0f},
            .type = Material::Type::Matte}} // Mesh 1 → metallic floor +
                                            // metallic wall (reflective
                                            // environment around the sphere)
  }};
};
