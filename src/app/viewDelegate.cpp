#include "viewDelegate.h"

#include "appDelegate.h"
#include "utils/devices.h"
#include <CoreGraphics/CGDirectDisplay.h>

ViewDelegate::ViewDelegate(MTL::Device *device, AppDelegate *appDelegate)
    : MTK::ViewDelegate(), m_renderer{new Renderer(device)},
      m_appDelegate{appDelegate} {}

ViewDelegate::~ViewDelegate() { delete m_renderer; }

void ViewDelegate::drawInMTKView(MTK::View *view) {
  // Detect if escape key was released between the previous 2 frames
  if (!isKeyDown(KeyCode::Escape) && m_isEscapeKeyDown)
    m_isEscaped = !m_isEscaped;
  m_isEscapeKeyDown = isKeyDown(KeyCode::Escape);

  if (m_isEscaped) {
    if (!m_isMouseShown) {
      CGDisplayShowCursor(CGMainDisplayID());
      m_isMouseShown = true;
    }
    return;
  }
  if (m_isMouseShown) {
    CGDisplayHideCursor(CGMainDisplayID());
    m_isMouseShown = false;
  }

  m_renderer->draw(view);
  m_appDelegate->updateTitleWithFPS();
}
