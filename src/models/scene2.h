#pragma once

#include "cpuObjects.h"

namespace Models {
const inline Model floor{
    .objects =
        {
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {-100.f, -2.f, 100.f},
                                .b = {100.f, -2.f, 100.f},
                                .c = {100.f, -2.f, -100.f},
                                .oneSided = false}},
            Object{.type = Object::Type::Triangle,
                   .triangle = {.a = {-100.f, -2.f, 100.f},
                                .b = {100.f, -2.f, -100.f},
                                .c = {-100.f, -2.f, -100.f},
                                .oneSided = false}},
        },
    .material = {.color = {0.5f, 0.5f, 0.5f},
                 .emissionStrength = 0.f,
                 .emissionColor = {0.f, 0.f, 0.f},
                 .type = Material::Type::Matte},
};

const inline Model sphere1{
    .objects = {Object{
        .type = Object::Type::Sphere,
        .sphere = Sphere{.center = {-6.f, 1.f, -1.f}, .radius = 2.5f}}},
    .material = {.color = {1.f, 0.2f, 0.2f},
                 .type = Material::Type::Metal,
                 .fuzz = 0.f},
};

const inline Model sphere2{
    .objects = {Object{.type = Object::Type::Sphere,
                       .sphere =
                           Sphere{.center = {-2.f, 3.f, 1.f}, .radius = 1.8f}}},
    .material = {.color = {0.2f, 1.f, 0.2f},
                 .type = Material::Type::Metal,
                 .fuzz = 0.1f},
};

const inline Model sphere3{
    .objects = {Object{.type = Object::Type::Sphere,
                       .sphere =
                           Sphere{.center = {2.f, 2.f, -2.f}, .radius = 2.2f}}},
    .material = {.color = {0.2f, 0.2f, 1.f}, .type = Material::Type::Matte},
};

const inline Model sphere4{
    .objects = {Object{.type = Object::Type::Sphere,
                       .sphere =
                           Sphere{.center = {5.f, 4.f, 0.f}, .radius = 1.9f}}},
    .material = {.color = {1.f, 0.2f, 1.f},
                 .type = Material::Type::Metal,
                 .fuzz = 1.f},
};
} // namespace Models
