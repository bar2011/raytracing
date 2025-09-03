#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model roofLight{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {1.f, 3.95f, -1.f},
                                .b = {-1.f, 3.95f, -1.f},
                                .c = {-1.f, 3.95f, 1.f},
                                .oneSided = false}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {1.f, 3.95f, 1.f},
                                .b = {1.f, 3.95f, -1.f},
                                .c = {-1.f, 3.95f, 1.f},
                                .oneSided = false}},
        },
    .material = {.color = {1.f, 1.f, 0.6f},
                 .emissionStrength = 1.0f,
                 .emissionColor = {1.f, 1.f, 0.6f},
                 .type = Material::Type::Matte},
};
}
