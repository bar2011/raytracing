#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model roof{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.f, 4.f, -2.f},
                                .b = {-2.f, 4.f, 2.f},
                                .c = {-2.f, 4.f, -2.f},
                                .oneSided = true}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.f, 4.f, 2.f},
                                .b = {-2.f, 4.f, 2.f},
                                .c = {2.f, 4.f, -2.f},
                                .oneSided = true}},
        },
    .material = {.color = {0.95f, 0.25f, 0.52f},
                 .emissionStrength = 0.f,
                 .emissionColor = {0.f, 0.f, 0.f},
                 .type = Material::Type::Matte},
};
}
