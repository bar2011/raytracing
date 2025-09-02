#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model floor{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {0.0f, 0.0f, 0.0f},
                                .b = {2.0f, 0.0f, 0.0f},
                                .c = {0.0f, 0.0f, 2.0f},
                                .oneSided = false}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.0f, 0.0f, 0.0f},
                                .b = {2.0f, 0.0f, 2.0f},
                                .c = {0.0f, 0.0f, 2.0f},
                                .oneSided = false}},
        },
    .material = {.color = {0.5f, 1.0f, 0.5f},
                 .emissionStrength = 0.0f,
                 .emissionColor = {0.0f, 0.0f, 0.0f},
                 .type = Material::Type::Matte},
};
}
