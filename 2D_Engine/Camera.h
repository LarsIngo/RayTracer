#pragma once

#include <glm/glm.hpp>

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
        void Update(float speed, float dt);

        // Position.
        glm::vec3 mPosition = glm::vec3(0.f, 0.f, 0.f);

        // Front.
        glm::vec3 mFrontDirection = glm::vec3(0.f, 0.f, 1.f);

        // Up.
        glm::vec3 mUpDirection = glm::vec3(0.f, 1.f, 0.f);

        // Right.
        glm::vec3 mRightDirection = glm::vec3(1.f, 0.f, 0.f);

        // Rotation in degrees.
        glm::vec3 mRotation = glm::vec3(0.f, 0.f, 0.f);

        // Scale.
        glm::vec3 mScale = glm::vec3(1.f, 1.f, 1.f);

        // Orientation matrix.
        glm::mat4 mOrientationMatrix;

        // View matrix.
        glm::mat4 mViewMatrix;

    private:
        glm::mat4 CalculateOrientationMatrix() const;
        glm::mat4 CalculateViewMatrix() const;
};
