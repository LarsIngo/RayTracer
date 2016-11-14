#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>

#include <glm/glm.hpp>
#include "RayTracer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Entity.h"
#include "Texture.h"

glm::vec3 WASDQEinput(float speed, const glm::vec3& front, const glm::vec3& up, const glm::vec3& right);
glm::vec2 Arrowinput(float speed);
float ZXinput(float speed);

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
    sceneBox->mDiffuseTexPath = L"assets/DefaultDiffuse.png";

    // Entities.
    Entity* model0 = scene.CreateEntity();
    Entity* model1 = scene.CreateEntity();
    model0->mPosition = glm::vec3(1.3f, -1.f, 0.f);
    model1->mPosition = glm::vec3(-1.3f, -1.f, 0.f);
    model0->mModel.Load("assets/OBJModel.obj");
    model1->mModel.Load("assets/OBJModel.obj");
    model0->mDiffuseTexPath = L"assets/DefaultNormal.png";
    model1->mDiffuseTexPath = L"assets/DefaultNormal.png";

    // Point lights.
    PointLight* pointLight = scene.CreatePointLight(); PointLight(glm::vec3(0.f, 0.f, -5.f), glm::vec3(1.f, 1.f, 1.f), 300.f);
    pointLight->pos = glm::vec3(0.f, 0.f, -1.f);
    pointLight->col = glm::vec3(1.f, 1.f, 1.f);
    pointLight->maxDistance = 300.f;

    // Create application.
    RayTracer rayTracer(640, 640, &scene);
    Renderer* renderer = rayTracer.mRenderer;

    while (rayTracer.Running()) {

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
            
        // Update.
        cam->Update(0.1f, 1.f);
        pointLight->pos += glm::vec3(arrowinput.x, 0.f, arrowinput.y);
        rayTracer.Update(scene);

        // Render.
        rayTracer.Render(scene);
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
