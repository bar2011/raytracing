#pragma once

#include "scene2.h"

#include "cpuObjects.h"
#include <functional>

namespace Models {
const inline std::vector<std::reference_wrapper<const Model>> allModels{
    floor, sphere1, sphere2, sphere3, sphere4};
// const inline std::vector<std::reference_wrapper<const Model>> allModels{
//     wall1, wall2,   wall3,   wall4,   roof,   light,
//     floor, object1, object2, object3, object4};

void getGPUmodels(std::vector<Object> &objects, std::vector<Mesh> &meshes);
} // namespace Models
