#pragma once

#include <glm/glm.hpp>

// Directional light.
struct DirectionalLight
{
    // Direction.
    glm::vec3 dir;
    // Color.
    glm::vec3 col;
    float pad[2];

    // Constructor.
    DirectionalLight();

    // Constructor.
    // dir Direction.
    // col Color.
    DirectionalLight(const glm::vec3& dir, const glm::vec3& col);
};

inline DirectionalLight::DirectionalLight()
{

}

inline DirectionalLight::DirectionalLight(const glm::vec3& dir, const glm::vec3& col)
{
    this->dir = dir;
    this->col = col;
}
