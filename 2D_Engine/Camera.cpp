#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>

Camera::Camera() 
{

}

Camera::~Camera() 
{

}

void Camera::Update(float speed, float dt)
{
    // Get movement & rotation.
    glm::vec3 movement = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);

    if (GetAsyncKeyState('W'))
    {
        movement += mFrontDirection;
    }
    if (GetAsyncKeyState('A'))
    {
        movement -= mRightDirection;
    }
    if (GetAsyncKeyState('S'))
    {
        movement -= mFrontDirection;
    }
    if (GetAsyncKeyState('D'))
    {
        movement += mRightDirection;
    }

    if (GetAsyncKeyState('Q'))
    {
        movement += mUpDirection;
    }

    if (GetAsyncKeyState('E'))
    {
        movement -= mUpDirection;
    }

    if (GetAsyncKeyState('Q'))
    {
        movement += mUpDirection;
    }

    if (GetAsyncKeyState('E'))
    {
        movement -= mUpDirection;
    }

    if (GetAsyncKeyState('X')) 
    {
        rotation -= glm::vec3(1.f, 0.f, 0.f);
    }

    if (GetAsyncKeyState('Z')) 
    {
        rotation += glm::vec3(1.f, 0.f, 0.f);
    }


    // Update postion & rotation.
    mPosition += movement * speed * dt;
    mRotation += rotation * speed * 20.f * dt;

    // Update direction vectors and matrices.
    mOrientationMatrix = CalculateOrientationMatrix();
    mFrontDirection = glm::vec4(0.f, 0.f, 1.f, 0.f) * mOrientationMatrix;
    mRightDirection = glm::vec4(1.f, 0.f, 0.f, 0.f) * mOrientationMatrix;
    mUpDirection = glm::vec4(0.f, 1.f, 0.f, 0.f) * mOrientationMatrix;
    mViewMatrix = CalculateViewMatrix();

}

glm::mat4 Camera::CalculateOrientationMatrix() const
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    return orientation;
}

glm::mat4 Camera::CalculateViewMatrix() const 
{
    return mOrientationMatrix * glm::translate(glm::mat4(), -mPosition);
}
