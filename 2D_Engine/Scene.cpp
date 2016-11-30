#include "Scene.h"

#include "SpotLight.h"
#include "Entity.h"
#include "Model.h"
#include "PointLight.h"
#include "Sphere.h"

Scene::Scene() 
{

}

Scene::~Scene() 
{
    for (Entity* e : mEntites)
        delete e;
    for (PointLight* p : mPointLights)
        delete p;
    for (SpotLight* d : mSpotLights)
        delete d;
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

SpotLight* Scene::CreateSpotLight()
{
    SpotLight* d = new SpotLight;
    mSpotLights.push_back(d);
    return d;
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

std::vector<SpotLight*>* Scene::GetSpotLights()
{
    return &mSpotLights;
}

std::vector<Sphere*>* Scene::GetSpheres()
{
    return &mSpheres;
}

std::size_t Scene::AddTexture(std::wstring path)
{
    const auto& it = mTextureIDMap.find(path);
    if (it != mTextureIDMap.end())
        return it->second;
    std::size_t index = mTextureIDMap.size();
    mTextureIDMap[path] = index;
    return index;
}
