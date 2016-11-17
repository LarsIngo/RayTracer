#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity()
{

}

Entity::~Entity() 
{
    mDiffuseTexPath.clear();
    mDiffuseTexPath.shrink_to_fit();

    mNormalTexPath.clear();
    mNormalTexPath.shrink_to_fit();
}

glm::mat4 Entity::CalculateModelMatrix() const
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    return glm::translate(glm::mat4(), mPosition) * orientation * glm::scale(glm::mat4(), mScale);
}
