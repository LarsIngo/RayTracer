#pragma once

#include "Camera.h"
#include <map>
#include <string>
#include <vector>

class Entity;
struct PointLight;

// A Scene.
class Scene 
{
    public:
        friend class RayTracer;

        // Constructor.
        Scene();

        // Destructor.
        ~Scene();

        // Create entity.
        // Returns pointer to entity.
        Entity* CreateEntity();

        // Create point light.
        // Returns pointer to point light.
        PointLight* CreatePointLight();

        // Get camera.
        // Returns pointer to camera.
        Camera* GetCamera();

        // Get entities.
        // Returns a vector of entities.
        std::vector<Entity*>* GetEntities();

        // Get point lights.
        // Returns a vector of point lights.
        std::vector<PointLight*>* GetPointLights();

    private:
        // Camera.
        Camera mCamera;

        // Entities.
        std::vector<Entity*> mEntites;

        // Point lights.
        std::vector<PointLight*> mPointLights;
};
