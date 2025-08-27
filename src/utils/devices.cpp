#include "devices.h"

#include <ApplicationServices/ApplicationServices.h>
#include <CoreGraphics/CGDirectDisplay.h>
#include <CoreGraphics/CoreGraphics.h>
#include <MetalKit/MetalKit.hpp>

std::pair<float, float> getViewSize(MTK::View *view) {
  auto drawableSize{view->drawableSize()};

  float scale{getPixelPointScale()};

  float width{static_cast<float>(drawableSize.width) / scale};
  float height{static_cast<float>(drawableSize.height) / scale};

  return {width, height};
}

std::pair<float, float> getDisplaySize() {
  CGDirectDisplayID mainDisplay{CGMainDisplayID()};
  CGRect displayBounds{CGDisplayBounds(mainDisplay)};
  return {CGRectGetWidth(displayBounds), CGRectGetHeight(displayBounds)};
}

void moveMouse(float x, float y) {
  CGEventRef moveEvt = CGEventCreateMouseEvent(
      nullptr, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft);

  CGEventPost(kCGHIDEventTap, moveEvt);
  CFRelease(moveEvt);
}

std::pair<float, float> getMousePos() {
  CGEventRef mousePosEvent = CGEventCreate(NULL);
  CGPoint mousePos = CGEventGetLocation(mousePosEvent);
  CFRelease(mousePosEvent);
  return {mousePos.x, mousePos.y};
}

bool isKeyDown(KeyCode key) {
  // kCGEventSourceStateCombinedSessionState checks keyboard state system-wide
  bool pressed = CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState,
                                       static_cast<unsigned int>(key));
  return pressed;
}

// Returns a map of every keycode, and whether it's pressed or not
std::unordered_map<KeyCode, bool> getKeysState() {
  std::unordered_map<KeyCode, bool> keysState{};

  for (const auto [name, keyCode] : keyCodeMap)
    keysState[keyCode] = isKeyDown(keyCode);

  return keysState;
}
