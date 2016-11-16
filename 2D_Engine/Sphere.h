#pragma once

#include <glm/glm.hpp>

// Sphere.
struct Sphere
{
    // Position.
    glm::vec3 pos;
    // Color.
    glm::vec3 col;
    // Radius.
    float radius;
    float pad;

    // Constructor.
    Sphere();

    // Constructor.
    // pos Position.
    // col Color.
    // radius Radius.
    Sphere(const glm::vec3& pos, const glm::vec3& col, float radius);
};

inline Sphere::Sphere()
{
    radius = 1.f;
}

inline Sphere::Sphere(const glm::vec3& pos, const glm::vec3& col, float radius)
{
    this->pos = pos;
    this->col = col;
    this->radius = radius;
}
