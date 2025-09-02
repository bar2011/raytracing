#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model roofLight{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {1.5f, 1.95f, 0.5f},
                                .b = {0.5f, 1.95f, 0.5f},
                                .c = {0.5f, 1.95f, 1.5f},
                                .oneSided = false}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {1.5f, 1.95f, 1.5f},
                                .b = {1.5f, 1.95f, 0.5f},
                                .c = {0.5f, 1.95f, 1.5f},
                                .oneSided = false}},
        },
    .material = {.color = {1.f, 1.f, 0.6f},
                 .emissionStrength = 1.0f,
                 .emissionColor = {1.f, 1.f, 0.6f},
                 .type = Material::Type::Matte},
};
}
