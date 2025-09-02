#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model centralObject{
    .objects =
        {
            Object{.type = Object::Type::Sphere,
                   .sphere = {.center = {1.f, 0.5f, 1.f}, .radius = 0.5f}},
        },
    .material = {.color = {0.6f, 0.6f, 0.6f},
                 .emissionStrength = 0.f,
                 .emissionColor = {0.f, 0.f, 0.f},
                 .type = Material::Type::Metal},
};
}
