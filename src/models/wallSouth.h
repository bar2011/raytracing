#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model wallSouth{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {0.0f, 0.0f, 2.0f},
                                .b = {2.0f, 0.0f, 2.0f},
                                .c = {0.0f, 2.0f, 2.0f},
                                .oneSided = true}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {0.0f, 2.0f, 2.0f},
                                .b = {2.0f, 0.0f, 2.0f},
                                .c = {2.0f, 2.0f, 2.0f},
                                .oneSided = true}},
        },
    .material = {.color = {0.3f, 0.3f, 1.0f},
                 .emissionStrength = 0.0f,
                 .emissionColor = {0.0f, 0.0f, 0.0f},
                 .type = Material::Type::Matte},
};
}
