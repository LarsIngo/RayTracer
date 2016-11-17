#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <chrono>
#include <crtdbg.h>
#include <iostream>
#include <glm/glm.hpp>
#include "DirectionalLight.h"
#include "DxAssert.h"
#include "Entity.h"
#include "PointLight.h"
#include "Profiler.h"
#include "RayTracer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Sphere.h"
#include "Texture.h"

glm::vec3 WASDQEinput(float speed, const glm::vec3& front, const glm::vec3& up, const glm::vec3& right);
glm::vec2 Arrowinput(float speed);
float ZXinput(float speed);

using namespace std::chrono;

int main() 
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Create scene.
    Scene scene;
    Camera* cam = scene.GetCamera();
    cam->mPosition.z = -5.f;

    // Scene box.
    Entity* sceneBox = scene.CreateEntity();
    sceneBox->mScale *= 20.f;
    sceneBox->mModel.Load("assets/OBJSceneBox.obj");
    sceneBox->mDiffuseTextureID = scene.AddTexture(L"assets/DiffuseBrick.png");
    //sceneBox->mNormalTextureID = scene.AddTexture(L"assets/NormalBrick.png");

    // Point lights.
    std::vector<PointLight*> pointLights;
    {
        PointLight* pointLight;
        pointLight = scene.CreatePointLight();
        pointLight->pos = glm::vec3(0.f, 0.f, -2.f);
        pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        pointLights.push_back(pointLight);

        //pointLight = scene.CreatePointLight();
        //pointLight->pos = glm::vec3(5.f, 0.f, -2.f);
        //pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        //pointLights.push_back(pointLight);

        //pointLight = scene.CreatePointLight();
        //pointLight->pos = glm::vec3(-5.f, 0.f, -2.f);
        //pointLight->col = glm::vec3(1.f, 1.f, 1.f);
        //pointLights.push_back(pointLight);
    }

    // Directional lights.
    {
        //DirectionalLight* directionalLight;
        //directionalLight = scene.CreateDirectionalLight();
        //directionalLight->dir = glm::normalize(glm::vec3(1.f, -2.f, 3.f));
        //directionalLight->col = glm::vec3(1.f, 1.f, 1.f);
    }

    // Spheres.
    {
        Sphere* sphere = scene.CreateSphere();
        sphere->pos = glm::vec3(0.f, 0.f, 0.f);
        sphere->col = glm::vec3(0.f, 0.f, 1.f);
        sphere->radius = 1.f;
    }

    // Entities.
    Entity* model0 = scene.CreateEntity();
    Entity* model1 = scene.CreateEntity();
    model0->mPosition = glm::vec3(1.3f, -1.f, 0.f);
    model1->mPosition = glm::vec3(-1.3f, -1.f, 0.f);
    model0->mModel.Load("assets/FBXModel.fbx");
    model1->mModel.Load("assets/OBJModel.obj");
    model0->mDiffuseTextureID = scene.AddTexture(L"assets/DiffuseBrick.png");
    model1->mDiffuseTextureID = scene.AddTexture(L"assets/DiffuseBrick.png");
    model0->mNormalTextureID = scene.AddTexture(L"assets/DefaultNormal.png");
    model1->mNormalTextureID = scene.AddTexture(L"assets/NormalBrick.png");

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
            
        // Camera.
        cam->Update(0.1f, 1.f, renderer);

        // Point lights.
        for (PointLight* pointLight : pointLights)
            pointLight->pos += glm::vec3(arrowinput.x, 0.f, arrowinput.y);

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

glm::vec3 WASDQEinput(float speed, const glm::vec3& front, const glm::vec3& up, const glm::vec3& right) {

    glm::vec3 input(0.f, 0.f, 0.f);

    if (GetAsyncKeyState('W'))
    {
        input += front;
    }
    if (GetAsyncKeyState('A'))
    {
        input -= right;
    }
    if (GetAsyncKeyState('S'))
    {
        input -= front;
    }
    if (GetAsyncKeyState('D'))
    {
        input += right;
    }

    if (GetAsyncKeyState('Q'))
    {
        input += up;
    }

    if (GetAsyncKeyState('E'))
    {
        input -= up;
    }

    return input * speed;

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
