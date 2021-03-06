#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Model.h"

// An entity.
class Entity 
{
    public:
        // Constructor.
        Entity();

        // Destructor.
        ~Entity();

        // Model matrix.
        glm::mat4 CalculateModelMatrix() const;

        // Position.
        glm::vec3 mPosition = glm::vec3(0.f, 0.f, 0.f);

        // Rotation.
        glm::vec3 mRotation = glm::vec3(0.f, 0.f, 0.f);

        // Scale.
        glm::vec3 mScale = glm::vec3(1.f, 1.f, 1.f);

        // Model.
        Model mModel;

        // Diffuse texture id.
        std::size_t mDiffuseTextureID = -1;

        // Normal texture id.
        std::size_t mNormalTextureID = -1;
};
