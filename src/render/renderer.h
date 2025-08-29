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

  std::array<Object, 5> m_objects{
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {0.f, 0.f, 1.f}, .radius = 1.f}},
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {0.f, 0.f, -5.f}, .radius = 1.f}},
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {2.f, 0.f, -6.f}, .radius = 0.5f}},
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {-1.5f, 1.f, -4.f}, .radius = 0.75f}},
      Object{.type = Object::Type::Sphere,
             .sphere = Sphere{.center = {3.f, 1.f, 4.f}, .radius = 2.f}},
  };
};
