#pragma once

#include "centralObject.h"
#include "floor.h"
#include "roof.h"
#include "roofLight.h"
#include "wallEast.h"
#include "wallNorth.h"
#include "wallSouth.h"
#include "wallWest.h"

#include <functional>

namespace Models {
const inline std::vector<std::reference_wrapper<const Model>> allModels{
    centralObject, floor,     roof,      roofLight,
    wallEast,      wallNorth, wallSouth, wallWest};

void getGPUmodels(std::vector<Object> &objects, std::vector<Mesh> &meshes);
} // namespace Models
