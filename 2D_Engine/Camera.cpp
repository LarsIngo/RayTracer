#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
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

    // Ray vs Plane.
    glm::vec3 rayOrigin = mPosition;
    glm::vec3 rayDirection = mFrontDirection;
    glm::vec3 v0 = glm::vec3(0.f, 0.f, 10.f);
    glm::vec3 v1 = glm::vec3(0.f, 20.f, 10.f);
    glm::vec3 v2 = glm::vec3(20.f, 0.f, 10.f);
    v0 = mViewMatrix * glm::vec4(v0, 1.f);
    v1 = mViewMatrix * glm::vec4(v1, 1.f);
    v2 = mViewMatrix * glm::vec4(v2, 1.f);
    rayOrigin = glm::vec3(0.f, 0.f, 1.f);
    rayDirection = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;
    glm::vec3 n = glm::normalize(glm::cross(e1, e2));
    float t = glm::dot(n, -rayDirection);
    if (t > 0.001f) {
        float d = -glm::dot(n, v0);
        t = (d + glm::dot(n, rayOrigin)) / t;
        if (t > -0.001f) 
        {
            std::cout << t << std::endl;
        }
    }

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
