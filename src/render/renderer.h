#pragma once

#include "Metal/MTLComputePipeline.hpp"
#include "Metal/MTLTexture.hpp"
#include "object.h"
#include "utils/camera.h"
#include <MetalKit/MetalKit.hpp>

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

  size_t m_iteration{};
  MTL::Texture *m_screenTexture{};

  std::array<Object, 6> m_objects{
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {0.f, -20.f, 0.f}, .radius = 20.f}},
      // Small sphere "sitting" on the ground sphere (at origin)
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {0.f, 1.f, 0.f}, .radius = 1.f}},

      // Medium sphere to the left
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {-2.f, 1.f, -3.f}, .radius = 2.f}},

      // Small sphere to the right
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {2.f, 0.5f, -2.f}, .radius = 0.5f}},

      // Larger sphere in the background
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {0.f, 3.f, -6.f}, .radius = 2.f}},

      // Tiny sphere floating above
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {1.5f, 4.f, -1.f}, .radius = 0.25f}}};
};
