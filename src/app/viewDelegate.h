#pragma once

#include "render/renderer.h"
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

class AppDelegate;

class ViewDelegate : public MTK::ViewDelegate {
public:
  ViewDelegate(MTL::Device *device, AppDelegate *appDelegate);
  virtual ~ViewDelegate() override;
  virtual void drawInMTKView(MTK::View *view) override;

private:
  Renderer *m_renderer;
  // Stores a reference to the app delegate
  AppDelegate *m_appDelegate;

  bool m_isEscapeKeyDown;
  bool m_isMouseShown{true};
  bool m_isEscaped{true};
};
