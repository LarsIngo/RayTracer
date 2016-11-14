#pragma once

#include <glm/glm.hpp>

// Point light.
struct PointLight 
{
    // Position.
    glm::vec3 pos;
    // Color.
    glm::vec3 col;
    // Max distance.
    float maxDistance;
    float pad;

    // Constructor.
    PointLight();

    // Constructor.
    // pos Position.
    // col Color.
    PointLight(const glm::vec3& pos, const glm::vec3& col, float maxDistance);
};

inline PointLight::PointLight() 
{

}

inline PointLight::PointLight(const glm::vec3& pos, const glm::vec3& col, float maxDistance)
{
    this->pos = pos;
    this->col = col;
    this->maxDistance = maxDistance;
}
