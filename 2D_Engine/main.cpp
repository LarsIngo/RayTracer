#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <chrono>
#include <crtdbg.h>
#include <iostream>
#include <glm/glm.hpp>
#include "SpotLight.h"
#include "DxAssert.h"
#include "Entity.h"
#include "PointLight.h"
#include "Profiler.h"
#include "RayTracer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Texture.h"

glm::vec2 Arrowinput(float speed);
float ZXinput(float speed);

using namespace std::chrono;

int main() 
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Create scene.
    Scene scene;
    Camera* cam = scene.GetCamera();
    //cam->mPosition = glm::vec3(-8.5f, 17.f, -8.f);
    cam->mPosition = glm::vec3(-25.f, 50.f, -15.f);
    cam->Yaw(45.f);
    cam->Pitch(45.f);
    //cam->mPosition = glm::vec3(0.f, 0.f, -5.f);

    // Scene box.
    Entity* sceneBox = scene.CreateEntity();
    sceneBox->mScale *= 20.f;
    sceneBox->mModel.Load("assets/INVBOX.obj");
    sceneBox->mDiffuseTextureID = scene.AddTexture(L"assets/DiffuseBrick.png");
    sceneBox->mNormalTextureID = scene.AddTexture(L"assets/NormalBrick.png");

    // Point lights.
    std::vector<PointLight*> pointLights;
    {
        float space = 7.f;

        PointLight* pointLight;
        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(0.f, 0.f, -space / 2.f);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(0.f, 0.f, space / 2.f);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(-space, space, space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(space, space, space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(-space, -space, space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(space, -space, space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(-space, space, -space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(space, space, -space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(-space, -space, -space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(space, -space, -space);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);
    }

    // Spot lights.
    {
        SpotLight* spotLight;
        spotLight = scene.CreateSpotLight();
        spotLight->pos = glm::vec3(0.f, 8.f, 0.0f);
        spotLight->col = glm::vec3(1.f, 1.f, 1.f);
        spotLight->dir = glm::vec3(0.f, -1.f, 0.f);
    }

    // Spheres.
    {
        Sphere* sphere;
        sphere = scene.CreateSphere();
        sphere->pos = glm::vec3(0.f, 0.f, 3.f);
        sphere->col = glm::vec3(0.f, 0.f, 1.f);
        sphere->radius = 1.f;

        sphere = scene.CreateSphere();
        sphere->pos = glm::vec3(5.f, 0.f, 0.f);
        sphere->col = glm::vec3(0.f, 1.f, 0.f);
        sphere->radius = 1.5f;

        sphere = scene.CreateSphere();
        sphere->pos = glm::vec3(-5.f, 0.f, -3.f);
        sphere->col = glm::vec3(1.f, 0.f, 0.f);
        sphere->radius = 2.5f;
    }

    // Entities.
    Entity* model0 = scene.CreateEntity();
    model0->mPosition = glm::vec3(0.f, 0.f, 0.f);
    model0->mModel.Load("assets/FBXAnimation.fbx");
    model0->mDiffuseTextureID = scene.AddTexture(L"assets/DiffuseBrick.png");
    model0->mNormalTextureID = scene.AddTexture(L"assets/NormalBrick.png");

    {
        for (int i = 0; i < 1; ++i) 
        {   // Loop to add more triangles.
            Entity* model = scene.CreateEntity();
            model->mPosition = glm::vec3(0.f, 0.f, -7.5f);
            model->mScale *= 2.f;
            model->mModel.Load("assets/FBXAnimation.fbx");
            model->mDiffuseTextureID = scene.AddTexture(L"assets/DiffuseBrick.png");
            model->mNormalTextureID = scene.AddTexture(L"assets/NormalBrick.png");
        }
    }

    // Create application.
    RayTracer rayTracer(640, 640, &scene);
    Renderer* renderer = rayTracer.mRenderer;

    // Set Frame Latency.
    IDXGIDevice1 * pDXGIDevice;
    DxAssert(renderer->mDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice), S_OK);
    DxAssert(pDXGIDevice->SetMaximumFrameLatency(1), S_OK);
    pDXGIDevice->Release();

    long long lastTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    float dt = 0.f;
    float duration = 0.f;
    while (rayTracer.Running()) {
        long long newTime = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        duration += dt = static_cast<float>(newTime - lastTime)/1000.f;
        lastTime = newTime;

        //std::cout << cam->mPosition.x << ":" << cam->mPosition.y << ":" << cam->mPosition.z << std::endl;

        // Input.
        float speed = 0.1f;
        glm::vec2 arrowinput = Arrowinput(speed);
        if (GetAsyncKeyState(VK_F1) && rayTracer.mNumBounces > 1)
        {
            --rayTracer.mNumBounces;
            std::cout << "Num Bounces: " << rayTracer.mNumBounces << std::endl;
        }

        if (GetAsyncKeyState(VK_F2) && rayTracer.mNumBounces < 6)
        {
            ++rayTracer.mNumBounces;
            std::cout << "Num Bounces: " << rayTracer.mNumBounces << std::endl;
        }

        if (GetAsyncKeyState(VK_F3) && rayTracer.mSSAA > 1)
        {
            rayTracer.mSSAA /= 2;
            std::cout << "SSAA: " << rayTracer.mSSAA << std::endl;
        }

        if (GetAsyncKeyState(VK_F4) && rayTracer.mSSAA < 4)
        {
            rayTracer.mSSAA *= 2;
            std::cout << "SSAA: " << rayTracer.mSSAA << std::endl;
        }

        if (GetAsyncKeyState(VK_F5) && rayTracer.mFOV > 3.14f / 8.f )
        {
            rayTracer.mFOV -= 0.5f * dt;
            std::cout << "FOV: " << rayTracer.mFOV << std::endl;
        }

        if (GetAsyncKeyState(VK_F6) && rayTracer.mFOV < 3.14f / 2.f)
        {
            rayTracer.mFOV += 0.5f * dt;
            std::cout << "FOV: " << rayTracer.mFOV << std::endl;
        }

        if (GetAsyncKeyState(VK_F7) && rayTracer.mEnergyCoefficient > 0.f)
        {
            rayTracer.mEnergyCoefficient -= 0.8f * dt;
            std::cout << "Energy coefficient: " << rayTracer.mEnergyCoefficient << std::endl;
        }

        if (GetAsyncKeyState(VK_F8) && rayTracer.mEnergyCoefficient < 1.f)
        {
            rayTracer.mEnergyCoefficient += 0.8f * dt;
            std::cout << "Energy coefficient: " << rayTracer.mEnergyCoefficient << std::endl;
        }
            
        // Camera.
        cam->Update(20.f, dt, renderer);

        // Point lights.
        for (PointLight* pointLight : pointLights)
            pointLight->pos += glm::vec3(arrowinput.x, 0.f, arrowinput.y) * dt * 20.f;

        // Models.
        model0->mModel.skeleton.Animate(&model0->mModel.animations[0], duration);
        model0->mModel.TransformMeshCPU();
        


        { PROFILE("GPU");
            rayTracer.Update(scene);
            rayTracer.Render(scene);
        }
    }

    return 0;

}


glm::vec2 Arrowinput(float speed) {

    glm::vec2 input(0.f, 0.f);

    if (GetAsyncKeyState(VK_UP))
    {
        input.y += speed;
    }
    if (GetAsyncKeyState(VK_LEFT))
    {
        input.x -= speed;
    }
    if (GetAsyncKeyState(VK_DOWN))
    {
        input.y -= speed;
    }
    if (GetAsyncKeyState(VK_RIGHT))
    {
        input.x += speed;
    }

    return input;

}

float ZXinput(float speed) {

    float input = 0.f;

    if (GetAsyncKeyState('Z'))
    {
        input += speed;
    }
    if (GetAsyncKeyState('X'))
    {
        input -= speed;
    }

    return input;

}
