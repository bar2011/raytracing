#pragma once

#include "Foundation/NSDate.hpp"
#include "app/viewDelegate.h"
#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class AppDelegate : public NS::ApplicationDelegate {
public:
  AppDelegate();
  ~AppDelegate();

  NS::Menu *createMenuBar();

  virtual void
  applicationWillFinishLaunching(NS::Notification *notification) override;
  virtual void
  applicationDidFinishLaunching(NS::Notification *notification) override;
  virtual bool applicationShouldTerminateAfterLastWindowClosed(
      NS::Application *sender) override;

  // Called every frame by view delegate
  void updateTitleWithFPS();

private:
  NS::Window *m_window;
  MTK::View *m_mtkView;
  MTL::Device *m_device;
  ViewDelegate *m_viewDelegate{nullptr};

  // For updating window title
  size_t m_framesSinceUpdate{};
  NS::Date *m_lastTime{NS::Date::dateWithTimeIntervalSinceNow(0.0)};
};
