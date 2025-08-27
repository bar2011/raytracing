#include "renderer.h"

#include "Foundation/NSString.hpp"
#include "Metal/MTLComputeCommandEncoder.hpp"
#include "Metal/MTLPixelFormat.hpp"
#include "Metal/MTLRenderCommandEncoder.hpp"
#include "Metal/MTLRenderPass.hpp"
#include "Metal/MTLTypes.hpp"
#include "app/constants.h"
#include <cstddef>
#include <stdexcept>

Renderer::Renderer(MTL::Device *device) : m_device{device->retain()} {
  m_commandQueue = m_device->newCommandQueue();

  buildShaders();
  buildTexture();
}

Renderer::~Renderer() {
  m_commandQueue->release();
  m_device->release();
}

void Renderer::draw(MTK::View *view) {
  m_iteration++;

  NS::AutoreleasePool *autoreleasePool{NS::AutoreleasePool::alloc()->init()};

  MTL::CommandBuffer *commandBuffer{m_commandQueue->commandBuffer()};
  MTL::RenderPassDescriptor *renderPass{view->currentRenderPassDescriptor()};

  auto computeEncoder{commandBuffer->computeCommandEncoder()};

  computeEncoder->setComputePipelineState(m_tracerPipeline);

  computeEncoder->setBytes(&m_iteration, sizeof(size_t), 0);
  computeEncoder->setTexture(m_screenTexture, 0);

  MTL::Size threadsPerGrid{AppConstants::WindowWidth,
                           AppConstants::WindowHeight, 1};
  MTL::Size threadsPerThreadgroup{
      m_tracerPipeline->maxTotalThreadsPerThreadgroup(), 1, 1};

  computeEncoder->dispatchThreads(threadsPerGrid, threadsPerThreadgroup);

  computeEncoder->endEncoding();

  auto renderEncoder{commandBuffer->renderCommandEncoder(renderPass)};

  renderEncoder->setRenderPipelineState(m_texturePipeline);

  renderEncoder->setFragmentTexture(m_screenTexture, 0);

  renderEncoder->drawPrimitives(MTL::PrimitiveTypeTriangle, 0ul, 6ul, 1ul);

  renderEncoder->endEncoding();

  commandBuffer->presentDrawable(view->currentDrawable());
  commandBuffer->commit();

  autoreleasePool->release();
}

void Renderer::buildShaders() {
  // Create library
  m_defaultLibrary = m_device->newDefaultLibrary();
  if (!m_defaultLibrary)
    throw std::runtime_error{"Error while creating default library"};

  NS::Error *error{nullptr};

  // Create tracer shader
  auto tracerFunction{m_defaultLibrary->newFunction(
      NS::String::string("tracer", NS::UTF8StringEncoding))};
  if (!tracerFunction)
    throw std::runtime_error{"Tracer function can't be found"};

  m_tracerPipeline = m_device->newComputePipelineState(tracerFunction, &error);
  tracerFunction->release();
  if (!m_tracerPipeline) {
    throw std::runtime_error{error->localizedDescription()->utf8String()};
  }

  // Create texture shader
  auto textureVertexFunction{m_defaultLibrary->newFunction(
      NS::String::string("vertexTexture", NS::UTF8StringEncoding))};
  if (!textureVertexFunction)
    throw std::runtime_error{"Vertex texture function can't be found"};

  auto textureFragmentFunction{m_defaultLibrary->newFunction(
      NS::String::string("fragmentTexture", NS::UTF8StringEncoding))};
  if (!textureFragmentFunction)
    throw std::runtime_error{"Fragment texture function can't be found"};

  auto texturePipelineDescriptor{
      MTL::RenderPipelineDescriptor::alloc()->init()};
  texturePipelineDescriptor->setVertexFunction(textureVertexFunction);
  texturePipelineDescriptor->setFragmentFunction(textureFragmentFunction);
  texturePipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(
      MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

  m_texturePipeline =
      m_device->newRenderPipelineState(texturePipelineDescriptor, &error);
  texturePipelineDescriptor->release();
  textureFragmentFunction->release();
  textureVertexFunction->release();
  if (!m_texturePipeline)
    throw std::runtime_error{error->localizedDescription()->utf8String()};
}

void Renderer::buildTexture() {
  auto textureDesc{MTL::TextureDescriptor::alloc()->init()};
  textureDesc->setTextureType(MTL::TextureType2D);
  textureDesc->setWidth(AppConstants::WindowWidth);
  textureDesc->setHeight(AppConstants::WindowHeight);
  textureDesc->setStorageMode(MTL::StorageModeManaged);
  textureDesc->setPixelFormat(MTL::PixelFormatRGBA16Float);
  textureDesc->setUsage(MTL::TextureUsageShaderWrite |
                        MTL::TextureUsageShaderRead |
                        MTL::TextureUsageRenderTarget);

  m_screenTexture = m_device->newTexture(textureDesc);

  textureDesc->release();
}
