#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "app/appDelegate.h"
#include "runtime/parser.h"
#include <AppKit/AppKit.hpp>
#include <cstring>
#include <iostream>

int main(int argc, char **argv) {
  if (argc >= 2) {
    if (std::strcmp(argv[1], "help") == 0) {
      std::cout
          << "Options:\n"
          << "  help               - show this menu\n"
          << "  parse <file path>  - parse OBJ file and output a header file\n"
          << "                       which can be added to src/models and "
             "connected\n"
          << "                       to src/models/allModels.h to include it "
             "in the scene\n"
          << "  no arguments       - run the program normally\n\n";
      return 0;
    } else if (std::strcmp(argv[1], "parse") == 0 && argc >= 3) {
      std::cout << "#pragma once\n\n#include \"cpuObjects.h\"\n\nnamespace "
                   "Models {\n";
      for (std::string model : parseOBJ(argv[2]))
        std::cout << model << "\n\n";
      std::cout << "}\n";

      return 0;
    } else {
      std::cerr << "Unknown option. Use 'help' to see available commands.\n";
      return 1;
    }
  }

  NS::AutoreleasePool *autoreleasePool{NS::AutoreleasePool::alloc()->init()};

  AppDelegate controller{};

  NS::Application *app{NS::Application::sharedApplication()};
  app->setDelegate(&controller);
  app->run();

  autoreleasePool->release();

  return 0;
}
