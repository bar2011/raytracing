#pragma once

#include "models/cpuObjects.h"
#include <string>
#include <vector>

// Formats float for in-code usage (whole numbers in the format of 4.f, etc.)
std::string formatFloat(float f);

// Given OBJ filename (doesn't have to have .obj extension), returns vector of
// models specified in the file, which can be pasted into the models library
std::vector<std::string> parseOBJ(const std::string &fileName);

// Formats faces vector to a usable model file which can be pasted into the
// models library
std::string createModelFile(const std::vector<Triangle> &faces,
                            std::string modelName);
