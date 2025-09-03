#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model floor{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {-2.f, 0.f, -2.f},
                                .b = {2.f, 0.f, -2.f},
                                .c = {-2.f, 0.f, 2.f},
                                .oneSided = false}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.f, 0.f, -2.f},
                                .b = {2.f, 0.f, 2.f},
                                .c = {-2.f, 0.f, 2.f},
                                .oneSided = false}},
        },
    .material = {.color = {0.5f, 1.0f, 0.5f},
                 .emissionStrength = 0.0f,
                 .emissionColor = {0.0f, 0.0f, 0.0f},
                 .type = Material::Type::Matte},
};
}
