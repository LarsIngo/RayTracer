#pragma once

#include <glm/glm.hpp>

// Vertex point.
struct Vertex 
{
    // Position.
    glm::vec3 pos;
    // Normal.
    glm::vec3 norm;
    // Tangent.
    glm::vec3 tang;
    // UV.
    glm::vec2 uv;
    // Diffuse texture ID.
    int diffTexID;
    // Normal texture ID.
    int normTexID;
    float pad[3];

    // Constructor.
    Vertex() { };
};
