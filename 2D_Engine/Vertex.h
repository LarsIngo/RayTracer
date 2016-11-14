#pragma once

#include <glm/glm.hpp>

// Vertex point.
struct Vertex 
{
    // Position.
    glm::vec3 pos;
    // Normal.
    glm::vec3 norm;
    // UV.
    glm::vec2 uv;

    // Constructor.
    Vertex();

    // Constructor.
    // pos Vertex position.
    // norm Vertex normal.
    // uv Vertex uv.
    Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv);
};

inline Vertex::Vertex() 
{

}

inline Vertex::Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec2& uv) 
{
    this->pos = pos;
    this->norm = norm;
    this->uv = uv;
}
