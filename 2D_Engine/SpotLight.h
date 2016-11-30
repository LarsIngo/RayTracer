#pragma once

#include <glm/glm.hpp>

// Spot light.
struct SpotLight
{
    // Position.
    glm::vec3 pos;
    // Color.
    glm::vec3 col;
    // Direction.
    glm::vec3 dir;
    // Cone (degrees).
    float cone = 20.f;
    float pad[2];
};
