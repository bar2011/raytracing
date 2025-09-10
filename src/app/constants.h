#pragma once

#include <string_view>

namespace AppConstants {
constexpr int WindowX{100};
constexpr int WindowY{100};
// w : h ~ 16 : 9
constexpr int WindowWidth{1000};
constexpr int WindowHeight{563};
constexpr std::string_view windowTitle{"Tracer"};
} // namespace AppConstants
