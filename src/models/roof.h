#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model roof{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.0f, 2.0f, 0.0f},
                                .b = {0.0f, 2.0f, 0.0f},
                                .c = {0.0f, 2.0f, 2.0f},
                                .oneSided = true}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.0f, 2.0f, 2.0f},
                                .b = {2.0f, 2.0f, 0.0f},
                                .c = {0.0f, 2.0f, 2.0f},
                                .oneSided = true}},
        },
    .material = {.color = {0.95f, 0.25f, 0.52f},
                 .emissionStrength = 0.0f,
                 .emissionColor = {0.0f, 0.0f, 0.0f},
                 .type = Material::Type::Matte},
};
}
