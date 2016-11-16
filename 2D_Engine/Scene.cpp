#include "Scene.h"

#include "Entity.h"
#include "Model.h"
#include "Sphere.h"
#include "PointLight.h"

Scene::Scene() 
{

}

Scene::~Scene() 
{
    for (Entity* e : mEntites)
        delete e;
    for (PointLight* p : mPointLights)
        delete p;
    for (Sphere* s : mSpheres)
        delete s;
}

Entity* Scene::CreateEntity()
{
    Entity* e = new Entity;
    mEntites.push_back(e);
    return e;
}

PointLight* Scene::CreatePointLight()
{
    PointLight* p = new PointLight;
    mPointLights.push_back(p);
    return p;
}

Sphere* Scene::CreateSphere()
{
    Sphere* s = new Sphere;
    mSpheres.push_back(s);
    return s;
}

Camera* Scene::GetCamera()
{
    return &mCamera;
}

std::vector<Entity*>* Scene::GetEntities()
{
    return &mEntites;
}

std::vector<PointLight*>* Scene::GetPointLights()
{
    return &mPointLights;
}

std::vector<Sphere*>* Scene::GetSpheres()
{
    return &mSpheres;
}
