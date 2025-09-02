#pragma once

#include "Metal/MTLComputePipeline.hpp"
#include "Metal/MTLTexture.hpp"
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
};
