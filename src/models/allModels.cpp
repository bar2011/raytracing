#include "allModels.h"

#include "cpuObjects.h"
#include <vector>

namespace Models {
void getGPUmodels(std::vector<Object> &objects, std::vector<Mesh> &meshes) {
  objects.clear();
  meshes.clear();

  for (const auto &model : allModels) {
    if (model.get().objects.empty())
      continue;

    meshes.push_back(
        Mesh{.objectFirst = static_cast<uint32_t>(objects.size()),
             .objectCount = static_cast<uint32_t>(model.get().objects.size()),
             .material = model.get().material});
    objects.insert(objects.end(), model.get().objects.cbegin(),
                   model.get().objects.cend());
  }
}
} // namespace Models
