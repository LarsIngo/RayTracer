#pragma once

#include "Camera.h"
#include <map>
#include <string>
#include <vector>

class Entity;
struct DirectionalLight;
struct PointLight;
struct Sphere;

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

        // Create directional light.
        // Returns pointer to directional light.
        DirectionalLight* CreateDirectionalLight();

        // Create sphere.
        // Returns pointer to sphere.
        Sphere* CreateSphere();

        // Get camera.
        // Returns pointer to camera.
        Camera* GetCamera();

        // Get entities.
        // Returns a vector of entities.
        std::vector<Entity*>* GetEntities();

        // Get point lights.
        // Returns a vector of point lights.
        std::vector<PointLight*>* GetPointLights();

        // Get directional lights.
        // Returns a vector of directional lights.
        std::vector<DirectionalLight*>* GetDirectionalLights();

        // Get spheres.
        // Returns a vector of spheres.
        std::vector<Sphere*>* GetSpheres();

        // Add texture.
        // path Path to file.
        // Return texture index.
        std::size_t AddTexture(std::wstring path);

    private:
        // Camera.
        Camera mCamera;

        // Entities.
        std::vector<Entity*> mEntites;

        // Point lights.
        std::vector<PointLight*> mPointLights;

        // Directional lights.
        std::vector<DirectionalLight*> mDirectionalLights;

        // Spheres.
        std::vector<Sphere*> mSpheres;

        // Texture id map.
        std::map<std::wstring, std::size_t> mTextureIDMap;
};
