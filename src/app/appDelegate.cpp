#include "appDelegate.h"

#include "Foundation/NSString.hpp"
#include "constants.h"

AppDelegate::AppDelegate() : NS::ApplicationDelegate{} {}

AppDelegate::~AppDelegate() {
  m_window->release();
  m_mtkView->release();
  m_device->release();
  m_lastTime->release();
  delete m_viewDelegate;
}

NS::Menu *AppDelegate::createMenuBar() {
  NS::Menu *pMainMenu = NS::Menu::alloc()->init();
  NS::MenuItem *pAppMenuItem = NS::MenuItem::alloc()->init();
  NS::Menu *pAppMenu = NS::Menu::alloc()->init(
      NS::String::string("Appname", NS::UTF8StringEncoding));

  NS::String *appName =
      NS::RunningApplication::currentApplication()->localizedName();
  NS::String *quitItemName = NS::String::string("Quit ", NS::UTF8StringEncoding)
                                 ->stringByAppendingString(appName);
  SEL quitCb = NS::MenuItem::registerActionCallback(
      "appQuit", [](void *, SEL, const NS::Object *pSender) {
        auto pApp = NS::Application::sharedApplication();
        pApp->terminate(pSender);
      });

  NS::MenuItem *pAppQuitItem = pAppMenu->addItem(
      quitItemName, quitCb, NS::String::string("q", NS::UTF8StringEncoding));
  pAppQuitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
  pAppMenuItem->setSubmenu(pAppMenu);

  NS::MenuItem *pWindowMenuItem = NS::MenuItem::alloc()->init();
  NS::Menu *pWindowMenu = NS::Menu::alloc()->init(
      NS::String::string("Window", NS::UTF8StringEncoding));

  SEL closeWindowCb = NS::MenuItem::registerActionCallback(
      "windowClose", [](void *, SEL, const NS::Object *) {
        auto pApp = NS::Application::sharedApplication();
        pApp->windows()->object<NS::Window>(0)->close();
      });
  NS::MenuItem *pCloseWindowItem = pWindowMenu->addItem(
      NS::String::string("Close Window", NS::UTF8StringEncoding), closeWindowCb,
      NS::String::string("w", NS::UTF8StringEncoding));
  pCloseWindowItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

  pWindowMenuItem->setSubmenu(pWindowMenu);

  pMainMenu->addItem(pAppMenuItem);
  pMainMenu->addItem(pWindowMenuItem);

  pAppMenuItem->release();
  pWindowMenuItem->release();
  pAppMenu->release();
  pWindowMenu->release();

  return pMainMenu->autorelease();
}

void AppDelegate::applicationWillFinishLaunching(
    NS::Notification *notification) {
  NS::Menu *menu{createMenuBar()};
  NS::Application *app{
      reinterpret_cast<NS::Application *>(notification->object())};
  app->setMainMenu(menu);
  app->setActivationPolicy(NS::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(
    NS::Notification *notification) {
  CGRect frame =
      (CGRect){{AppConstants::WindowX, AppConstants::WindowY},
               {AppConstants::WindowWidth, AppConstants::WindowHeight}};

  m_window = NS::Window::alloc()->init(
      frame, NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
      NS::BackingStoreBuffered, false);

  m_device = MTL::CreateSystemDefaultDevice();

  m_mtkView = MTK::View::alloc()->init(frame, m_device);
  m_mtkView->setColorPixelFormat(MTL::PixelFormatBGRA8Unorm_sRGB);
  m_mtkView->setClearColor(MTL::ClearColor::Make(0.3, 0.3, 0.3, 1.0));
  m_mtkView->setFramebufferOnly(false);

  m_viewDelegate = new ViewDelegate(m_device, this);
  m_mtkView->setDelegate(m_viewDelegate);

  m_mtkView->setPaused(false);
  m_mtkView->setPreferredFramesPerSecond(1000000);

  m_window->setContentView(m_mtkView);
  m_window->setTitle(NS::String::string(AppConstants::windowTitle.data(),
                                        NS::UTF8StringEncoding));
  m_window->makeKeyAndOrderFront(nullptr);

  NS::Application *app{
      reinterpret_cast<NS::Application *>(notification->object())};
  app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(
    NS::Application *sender) {
  return true;
}

void AppDelegate::updateTitleWithFPS() {
  m_framesSinceUpdate++;
  auto now{NS::Date::dateWithTimeIntervalSinceNow(0.0)};
  auto difference{now->timeIntervalSinceDate(m_lastTime)};

  // Update title only every 0.1s to avoid flickering
  if (difference >= 0.1) {
    auto fps{m_framesSinceUpdate / difference};
    m_window->setTitle(
        NS::String::string((std::string{AppConstants::windowTitle} + " (" +
                            std::to_string(fps) + "FPS)")
                               .data(),
                           NS::UTF8StringEncoding));
    m_lastTime->release();
    m_lastTime = now;
    m_framesSinceUpdate = 0;
  }
}
