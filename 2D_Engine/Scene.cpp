#include "Scene.h"

#include "Entity.h"
#include "Model.h"
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
