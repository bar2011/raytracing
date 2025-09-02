#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model wallNorth{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {0.0f, 0.0f, 0.0f},
                                .b = {0.0f, 2.0f, 0.0f},
                                .c = {2.0f, 0.0f, 0.0f},
                                .oneSided = true}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {0.0f, 2.0f, 0.0f},
                                .b = {2.0f, 2.0f, 0.0f},
                                .c = {2.0f, 0.0f, 0.0f},
                                .oneSided = true}},
        },
    .material = {.color = {1.0f, 0.3f, 0.3f},
                 .emissionStrength = 0.0f,
                 .emissionColor = {0.0f, 0.0f, 0.0f},
                 .type = Material::Type::Matte},
};
}
