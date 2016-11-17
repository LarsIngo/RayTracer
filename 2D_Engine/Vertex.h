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
    float pad;

    // Constructor.
    Vertex();

    // Constructor.
    // pos Vertex position.
    // norm Vertex normal.
    // tang Vertex tangent.
    // uv Vertex uv.
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& tang, const glm::vec2& uv);
};

inline Vertex::Vertex() 
{

}

inline Vertex::Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& tang, const glm::vec2& uv)
{
    this->pos = pos;
    this->norm = norm;
    this->tang = tang;
    this->uv = uv;
}
