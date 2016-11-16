#pragma once

#include <glm/glm.hpp>

class Renderer;

// A camera.
class Camera 
{
    public:
        // Constructor.
        Camera();

        // Destructor.
        ~Camera();

        // Update camera attributes.
        // speed Speed of movement.
        // dt Delta time.
        // render Parse renderer to get info about windwow.
        void Update(float speed, float dt, Renderer* render);

        // Rotate camera around up direction.
        // rotation Amount of rotation in degrees.
        void Yaw(float rotation);

        // Rotate camera around right direction.
        // rotation Amount of rotation in degrees.
        void Pitch(float rotation);

        // Rotate camera around front direction.
        // rotation Amount of rotation in degrees.
        void Roll(float rotation);

        // Position.
        glm::vec3 mPosition = glm::vec3(0.f, 0.f, 0.f);

        // Front.
        glm::vec3 mFrontDirection = glm::vec3(0.f, 0.f, 1.f);

        // Up.
        glm::vec3 mUpDirection = glm::vec3(0.f, 1.f, 0.f);

        // Right.
        glm::vec3 mRightDirection = glm::vec3(1.f, 0.f, 0.f);

        // Scale.
        glm::vec3 mScale = glm::vec3(1.f, 1.f, 1.f);

        // Orientation matrix.
        glm::mat4 mOrientationMatrix;

        // View matrix.
        glm::mat4 mViewMatrix;

        // Old mouse left button pressed status.
        bool mOldMouseLeftButtonPressed = false;

        // Old mouse position.
        glm::vec2 mOldMousePosition = glm::vec2(0.f, 0.f);

        // New mouse position.
        glm::vec2 mNewMousePosition = glm::vec2(0.f, 0.f);

    private:
        glm::mat4 CalculateOrientationMatrix() const;
        glm::mat4 CalculateViewMatrix() const;
};
