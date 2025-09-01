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

  std::array<Object, 15> m_objects{
      // Horizontal floor plane triangles (light green, double-sided)
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 0.0f, 0.0f},
                          .b = {2.0f, 0.0f, 0.0f},
                          .c = {0.0f, 0.0f, 2.0f},
                          .oneSided = false}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {2.0f, 0.0f, 0.0f},
                          .b = {2.0f, 0.0f, 2.0f},
                          .c = {0.0f, 0.0f, 2.0f},
                          .oneSided = false}},

      // North wall (z=0, facing +z direction) - red
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 0.0f, 0.0f},
                          .b = {0.0f, 2.0f, 0.0f},
                          .c = {2.0f, 0.0f, 0.0f},
                          .oneSided = true}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 2.0f, 0.0f},
                          .b = {2.0f, 2.0f, 0.0f},
                          .c = {2.0f, 0.0f, 0.0f},
                          .oneSided = true}},

      // South wall (z=2, facing -z direction) - blue
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 0.0f, 2.0f},
                          .b = {2.0f, 0.0f, 2.0f},
                          .c = {0.0f, 2.0f, 2.0f},
                          .oneSided = true}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 2.0f, 2.0f},
                          .b = {2.0f, 0.0f, 2.0f},
                          .c = {2.0f, 2.0f, 2.0f},
                          .oneSided = true}},

      // West wall (x=0, facing +x direction) - yellow
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 0.0f, 0.0f},
                          .b = {0.0f, 0.0f, 2.0f},
                          .c = {0.0f, 2.0f, 0.0f},
                          .oneSided = true}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {0.0f, 0.0f, 2.0f},
                          .b = {0.0f, 2.0f, 2.0f},
                          .c = {0.0f, 2.0f, 0.0f},
                          .oneSided = true}},

      // East wall (x=2, facing -x direction) - purple
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {2.0f, 0.0f, 0.0f},
                          .b = {2.0f, 2.0f, 0.0f},
                          .c = {2.0f, 0.0f, 2.0f},
                          .oneSided = true}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {2.0f, 0.0f, 2.0f},
                          .b = {2.0f, 2.0f, 0.0f},
                          .c = {2.0f, 2.0f, 2.0f},
                          .oneSided = true}},

      // Horizontal roof plane triangles - pink
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {2.0f, 2.0f, 0.0f},
                          .b = {0.0f, 2.0f, 0.0f},
                          .c = {0.0f, 2.0f, 2.0f},
                          .oneSided = true}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {2.0f, 2.0f, 2.0f},
                          .b = {2.0f, 2.0f, 0.0f},
                          .c = {0.0f, 2.0f, 2.0f},
                          .oneSided = true}},

      // Roof light
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {1.5f, 1.95f, 0.5f},
                          .b = {0.5f, 1.95f, 0.5f},
                          .c = {0.5f, 1.95f, 1.5f},
                          .oneSided = false}},
      Object{.type = Object::Type::Triangle,
             .triangle = {.a = {1.5f, 1.95f, 1.5f},
                          .b = {1.5f, 1.95f, 0.5f},
                          .c = {0.5f, 1.95f, 1.5f},
                          .oneSided = false}},

      // Metal silver sphere
      Object{.type = Object::Type::Sphere,
             .sphere = {.center = {1.f, 0.5f, 1.f}, .radius = 0.5f}},
  };

  std::array<Mesh, 8> m_meshes{
      // Horizontal plane (light green)
      Mesh{.objectFirst = 0,
           .objectCount = 2,
           .material = {.color = {0.5f, 1.0f, 0.5f},
                        .emissionStrength = 0.0f,
                        .emissionColor = {0.0f, 0.0f, 0.0f},
                        .type = Material::Type::Matte}},

      // North wall (red)
      Mesh{.objectFirst = 2,
           .objectCount = 2,
           .material = {.color = {1.0f, 0.3f, 0.3f},
                        .emissionStrength = 0.0f,
                        .emissionColor = {0.0f, 0.0f, 0.0f},
                        .type = Material::Type::Matte}},

      // South wall (blue)
      Mesh{.objectFirst = 4,
           .objectCount = 2,
           .material = {.color = {0.3f, 0.3f, 1.0f},
                        .emissionStrength = 0.0f,
                        .emissionColor = {0.0f, 0.0f, 0.0f},
                        .type = Material::Type::Matte}},

      // West wall (yellow)
      Mesh{.objectFirst = 6,
           .objectCount = 2,
           .material = {.color = {1.0f, 1.0f, 0.3f},
                        .emissionStrength = 0.0f,
                        .emissionColor = {0.0f, 0.0f, 0.0f},
                        .type = Material::Type::Matte}},

      // East wall (purple)
      Mesh{.objectFirst = 8,
           .objectCount = 2,
           .material = {.color = {0.8f, 0.3f, 0.8f},
                        .emissionStrength = 0.0f,
                        .emissionColor = {0.0f, 0.0f, 0.0f},
                        .type = Material::Type::Matte}},

      // Roof (pink)
      Mesh{.objectFirst = 10,
           .objectCount = 2,
           .material = {.color = {0.95f, 0.25f, 0.52f},
                        .emissionStrength = 0.0f,
                        .emissionColor = {0.0f, 0.0f, 0.0f},
                        .type = Material::Type::Matte}},

      // Roof light
      Mesh{.objectFirst = 12,
           .objectCount = 2,
           .material = {.color = {1.f, 1.f, 0.6f},
                        .emissionStrength = 1.0f,
                        .emissionColor = {1.f, 1.f, 0.6f},
                        .type = Material::Type::Matte}},

      // Sphere
      Mesh{.objectFirst = 14,
           .objectCount = 1,
           .material = {.color = {0.6f, 0.6f, 0.6f},
                        .emissionStrength = 0.f,
                        .emissionColor = {0.f, 0.f, 0.f},
                        .type = Material::Type::Metal}},
  };
};
