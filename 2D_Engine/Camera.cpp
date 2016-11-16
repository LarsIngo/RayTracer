#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <Windows.h>
#include "Renderer.h"

Camera::Camera() 
{

}

Camera::~Camera() 
{

}

void Camera::Update(float speed, float dt, Renderer* renderer)
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
        rotation -= glm::vec3(0.f, 0.f, 1.f);
    }

    if (GetAsyncKeyState('Z')) 
    {
        rotation += glm::vec3(0.f, 0.f, 1.f);
    }

    if (renderer->GetMouseInsideWindow())
    {
        mOldMousePosition = mNewMousePosition;
        mNewMousePosition = renderer->GetMousePosition();
        if (renderer->GetMouseLeftButtonPressed()) {
            glm::vec2 delta = mNewMousePosition - mOldMousePosition;
            rotation += glm::vec3(delta.x, -delta.y, 0.f) * 15.f;
        }
    }

    // Update postion & rotation.
    mPosition += movement * speed * dt;

    // Update direction vectors and matrices.
    Roll(rotation.z * speed * dt);
    Pitch(rotation.y * speed * dt);
    Yaw(rotation.x * speed * dt);

    mOrientationMatrix = CalculateOrientationMatrix();
    mViewMatrix = CalculateViewMatrix();
}

void Camera::Yaw(float rotation)
{
    glm::quat q = glm::angleAxis(rotation, mUpDirection);
    mRightDirection = glm::normalize(q * mRightDirection);
    mFrontDirection = glm::normalize(q * mFrontDirection);
    
}

void Camera::Pitch(float rotation)
{
    glm::quat q = glm::angleAxis(rotation, mRightDirection);
    mFrontDirection = glm::normalize(q * mFrontDirection);
    mUpDirection = glm::normalize(q * mUpDirection);
}

void Camera::Roll(float rotation)
{
    glm::quat q = glm::angleAxis(rotation, mFrontDirection);
    mUpDirection = glm::normalize(q * mUpDirection);
    mRightDirection = glm::normalize(q * mRightDirection);
}

glm::mat4 Camera::CalculateOrientationMatrix() const
{
    //glm::mat4 orientation;
    //orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    //orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    //orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    //return orientation;
    return glm::transpose(glm::mat4(
        glm::vec4(mRightDirection, 0.f),
        glm::vec4(mUpDirection, 0.f),
        glm::vec4(mFrontDirection, 0.f),
        glm::vec4(0.f, 0.f, 0.f, 1.f)
    ));
}

glm::mat4 Camera::CalculateViewMatrix() const 
{
    return mOrientationMatrix * glm::translate(glm::mat4(), -mPosition);
}
