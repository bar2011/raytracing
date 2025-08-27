#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "app/appDelegate.h"
#include <AppKit/AppKit.hpp>

int main() {
  NS::AutoreleasePool *autoreleasePool{NS::AutoreleasePool::alloc()->init()};

  AppDelegate controller{};

  NS::Application *app{NS::Application::sharedApplication()};
  app->setDelegate(&controller);
  app->run();

  autoreleasePool->release();

  return 0;
}
