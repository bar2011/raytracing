#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model wallEast{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.0f, 0.0f, 0.0f},
                                .b = {2.0f, 2.0f, 0.0f},
                                .c = {2.0f, 0.0f, 2.0f},
                                .oneSided = true}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {2.0f, 0.0f, 2.0f},
                                .b = {2.0f, 2.0f, 0.0f},
                                .c = {2.0f, 2.0f, 2.0f},
                                .oneSided = true}},
        },
    .material = {.color = {0.8f, 0.3f, 0.8f},
                 .emissionStrength = 0.0f,
                 .emissionColor = {0.0f, 0.0f, 0.0f},
                 .type = Material::Type::Matte},
};
}
