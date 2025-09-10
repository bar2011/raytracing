#include "models/cpuObjects.h"
#include "parser.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <simd/simd.h>
#include <sstream>
#include <string>
#include <vector>

std::string formatFloat(float f) {
  if (std::floor(f) == f) // whole number
    return std::to_string(static_cast<int>(f)) + ".f";
  else {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << f
        << "f"; // adjust precision as needed
    return oss.str();
  }
}

std::vector<std::string> parseOBJ(const std::string &fileName) {
  std::ifstream file{fileName};

  std::vector<simd::float3> vertices{};
  std::vector<Triangle> faces{};

  std::vector<std::string> resultModelVars{};

  std::string currentModelName{"modelname"};

  std::string line{};
  while (std::getline(file, line)) {
    std::istringstream lineSS{line};
    if (line.starts_with("o ")) {
      // If vertices/faces exist, create a model file from them before moving to
      // the next model
      if (faces.size() > 0)
        resultModelVars.push_back(createModelVar(faces, currentModelName));

      std::string prefix;
      lineSS >> prefix >> currentModelName;
      faces.clear();
    } else if (line.starts_with("v ")) {
      std::string prefix;
      std::array<float, 4> v{};
      if (!(lineSS >> prefix >> v[0] >> v[1] >> v[2]))
        std::cerr << "Failed to parse vertex line: " << line << '\n';
      else {
        if (lineSS >> v[3]) {
          v[0] /= v[3];
          v[1] /= v[3];
          v[2] /= v[3];
        }
        vertices.push_back(simd::float3{v[0], v[1], v[2]});
      }
    } else if (line.starts_with("f ")) {
      std::vector<std::size_t> verticesIndices{};

      std::size_t v{};
      lineSS.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
      while (lineSS >> v) {
        verticesIndices.push_back(v - 1);
        if (verticesIndices.size() >= 3)
          faces.push_back(Triangle{
              .a = vertices[verticesIndices[0]],
              .b = vertices[verticesIndices[verticesIndices.size() - 2]],
              .c = vertices[verticesIndices[verticesIndices.size() - 1]],
              .oneSided = false,
          });
        lineSS.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
      }
    }
  }

  // Create final model file
  if (faces.size() > 0)
    resultModelVars.push_back(createModelVar(faces, currentModelName));

  return resultModelVars;
}

std::string createModelVar(const std::vector<Triangle> &faces,
                           std::string modelName) {
  std::ostringstream result{};

  result << "const inline Model " << modelName << " {\n\t.objects =\n\t\t{";

  for (const auto &t : faces)
    result << "\n\t\t\tObject{\n\t\t\t\t.type = Object::Type::Triangle,\n"
              "\t\t\t\t.triangle = {.a = {"
           << formatFloat(t.a.x) << ", " << formatFloat(t.a.y) << ", "
           << formatFloat(t.a.z) << "},\n"
           << "\t\t\t\t\t.b = {" << formatFloat(t.b.x) << ", "
           << formatFloat(t.b.y) << ", " << formatFloat(t.b.z) << "},\n"
           << "\t\t\t\t\t.c = {" << formatFloat(t.c.x) << ", "
           << formatFloat(t.c.y) << ", " << formatFloat(t.c.z) << "},\n"
           << "\t\t\t\t\t.oneSided = " << (t.oneSided ? "true" : "false")
           << "}},";

  result << "\n\t\t},\n\t.material = {\n\t\t.color = {0.f, 0.f, "
            "0.f},\n\t\t.emissionStrength = 0.f,\n\t\t.emissionColor = {0.f, "
            "0.f, 0.f},\n\t\t.type = Material::Type::Matte},\n};";

  return result.str();
}
