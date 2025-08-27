#pragma once

#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <string>
#include <unordered_map>
#include <utility>

// Get pixel per point scale for current display
extern "C" float getPixelPointScale();

// Returns view size *in points*
std::pair<float, float> getViewSize(MTK::View *view);

// Returns display size *in points*
std::pair<float, float> getDisplaySize();

void moveMouse(float x, float y);

std::pair<float, float> getMousePos();

enum class KeyCode : unsigned int;

bool isKeyDown(KeyCode key);

std::unordered_map<KeyCode, bool> getKeysState();

enum class KeyCode : unsigned int {
  // Letters A-Z
  A = 0x00,
  B = 0x0B,
  C = 0x08,
  D = 0x02,
  E = 0x0E,
  F = 0x03,
  G = 0x05,
  H = 0x04,
  I = 0x22,
  J = 0x26,
  K = 0x28,
  L = 0x25,
  M = 0x2E,
  N = 0x2D,
  O = 0x1F,
  P = 0x23,
  Q = 0x0C,
  R = 0x0F,
  S = 0x01,
  T = 0x11,
  U = 0x20,
  V = 0x09,
  W = 0x0D,
  X = 0x07,
  Y = 0x10,
  Z = 0x06,

  // Numbers 0-9
  Num0 = 0x1D,
  Num1 = 0x12,
  Num2 = 0x13,
  Num3 = 0x14,
  Num4 = 0x15,
  Num5 = 0x17,
  Num6 = 0x16,
  Num7 = 0x1A,
  Num8 = 0x1C,
  Num9 = 0x19,

  // Punctuation & Symbols
  Equal = 0x18,
  Minus = 0x1B,
  RightBracket = 0x1E,
  LeftBracket = 0x21,
  Quote = 0x27,
  Semicolon = 0x29,
  Backslash = 0x2A,
  Comma = 0x2B,
  Slash = 0x2C,
  Period = 0x2F,
  Grave = 0x32,

  // Keypad 0-9
  Keypad0 = 0x52,
  Keypad1 = 0x53,
  Keypad2 = 0x54,
  Keypad3 = 0x55,
  Keypad4 = 0x56,
  Keypad5 = 0x57,
  Keypad6 = 0x58,
  Keypad7 = 0x59,
  Keypad8 = 0x5B,
  Keypad9 = 0x5C,
  KeypadDecimal = 0x41,
  KeypadMultiply = 0x43,
  KeypadPlus = 0x45,
  KeypadClear = 0x47,
  KeypadDivide = 0x4B,
  KeypadEnter = 0x4C,
  KeypadMinus = 0x4E,
  KeypadEquals = 0x51,

  // Controls
  Return = 0x24,
  Tab = 0x30,
  Space = 0x31,
  Delete = 0x33,
  Escape = 0x35,
  Command = 0x37,
  Shift = 0x38,
  CapsLock = 0x39,
  Option = 0x3A,
  Control = 0x3B,
  RightShift = 0x3C,
  RightOption = 0x3D,
  RightControl = 0x3E,
  Function = 0x3F,
  Help = 0x72,
  ForwardDelete = 0x75,

  // Navigation
  LeftArrow = 0x7B,
  RightArrow = 0x7C,
  DownArrow = 0x7D,
  UpArrow = 0x7E,
  Home = 0x73,
  End = 0x77,
  PageUp = 0x74,
  PageDown = 0x79,

  // Function keys F1-F20
  F1 = 0x7A,
  F2 = 0x78,
  F3 = 0x63,
  F4 = 0x76,
  F5 = 0x60,
  F6 = 0x61,
  F7 = 0x62,
  F8 = 0x64,
  F9 = 0x65,
  F10 = 0x6D,
  F11 = 0x67,
  F12 = 0x6F,
  F13 = 0x69,
  F14 = 0x6B,
  F15 = 0x71,
  F16 = 0x6A,
  F17 = 0x40,
  F18 = 0x4F,
  F19 = 0x50,
  F20 = 0x5A,

  // Audio
  VolumeUp = 0x48,
  VolumeDown = 0x49,
  Mute = 0x4A,
};

inline std::unordered_map<std::string, KeyCode> keyCodeMap = {
    // Letters
    {"A", KeyCode::A},
    {"B", KeyCode::B},
    {"C", KeyCode::C},
    {"D", KeyCode::D},
    {"E", KeyCode::E},
    {"F", KeyCode::F},
    {"G", KeyCode::G},
    {"H", KeyCode::H},
    {"I", KeyCode::I},
    {"J", KeyCode::J},
    {"K", KeyCode::K},
    {"L", KeyCode::L},
    {"M", KeyCode::M},
    {"N", KeyCode::N},
    {"O", KeyCode::O},
    {"P", KeyCode::P},
    {"Q", KeyCode::Q},
    {"R", KeyCode::R},
    {"S", KeyCode::S},
    {"T", KeyCode::T},
    {"U", KeyCode::U},
    {"V", KeyCode::V},
    {"W", KeyCode::W},
    {"X", KeyCode::X},
    {"Y", KeyCode::Y},
    {"Z", KeyCode::Z},

    // Numbers 0-9 (normal keys)
    {"0", KeyCode::Num0},
    {"1", KeyCode::Num1},
    {"2", KeyCode::Num2},
    {"3", KeyCode::Num3},
    {"4", KeyCode::Num4},
    {"5", KeyCode::Num5},
    {"6", KeyCode::Num6},
    {"7", KeyCode::Num7},
    {"8", KeyCode::Num8},
    {"9", KeyCode::Num9},

    // Numbers K0-K9 (keypad)
    {"K0", KeyCode::Keypad0},
    {"K1", KeyCode::Keypad1},
    {"K2", KeyCode::Keypad2},
    {"K3", KeyCode::Keypad3},
    {"K4", KeyCode::Keypad4},
    {"K5", KeyCode::Keypad5},
    {"K6", KeyCode::Keypad6},
    {"K7", KeyCode::Keypad7},
    {"K8", KeyCode::Keypad8},
    {"K9", KeyCode::Keypad9},

    // Punctuation
    {"=", KeyCode::Equal},
    {"-", KeyCode::Minus},
    {"]", KeyCode::RightBracket},
    {"[", KeyCode::LeftBracket},
    {"'", KeyCode::Quote},
    {";", KeyCode::Semicolon},
    {"\\", KeyCode::Backslash},
    {",", KeyCode::Comma},
    {"/", KeyCode::Slash},
    {".", KeyCode::Period},
    {"`", KeyCode::Grave},

    // Controls
    {"Return", KeyCode::Return},
    {"Tab", KeyCode::Tab},
    {"Space", KeyCode::Space},
    {"Delete", KeyCode::Delete},
    {"Escape", KeyCode::Escape},
    {"Command", KeyCode::Command},
    {"Shift", KeyCode::Shift},
    {"CapsLock", KeyCode::CapsLock},
    {"Option", KeyCode::Option},
    {"Control", KeyCode::Control},
    {"RightShift", KeyCode::RightShift},
    {"RightOption", KeyCode::RightOption},
    {"RightControl", KeyCode::RightControl},
    {"Function", KeyCode::Function},
    {"Help", KeyCode::Help},
    {"ForwardDelete", KeyCode::ForwardDelete},

    // Navigation
    {"LeftArrow", KeyCode::LeftArrow},
    {"RightArrow", KeyCode::RightArrow},
    {"DownArrow", KeyCode::DownArrow},
    {"UpArrow", KeyCode::UpArrow},
    {"Home", KeyCode::Home},
    {"End", KeyCode::End},
    {"PageUp", KeyCode::PageUp},
    {"PageDown", KeyCode::PageDown},

    // Function keys
    {"F1", KeyCode::F1},
    {"F2", KeyCode::F2},
    {"F3", KeyCode::F3},
    {"F4", KeyCode::F4},
    {"F5", KeyCode::F5},
    {"F6", KeyCode::F6},
    {"F7", KeyCode::F7},
    {"F8", KeyCode::F8},
    {"F9", KeyCode::F9},
    {"F10", KeyCode::F10},
    {"F11", KeyCode::F11},
    {"F12", KeyCode::F12},
    {"F13", KeyCode::F13},
    {"F14", KeyCode::F14},
    {"F15", KeyCode::F15},
    {"F16", KeyCode::F16},
    {"F17", KeyCode::F17},
    {"F18", KeyCode::F18},
    {"F19", KeyCode::F19},
    {"F20", KeyCode::F20},

    // Audio
    {"VolumeUp", KeyCode::VolumeUp},
    {"VolumeDown", KeyCode::VolumeDown},
    {"Mute", KeyCode::Mute},
};
