#include "RayTracer.h"

#include <d3d11.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "DirectionalLight.h"
#include "Entity.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Sphere.h"
#include "SkinVertex.h"
#include "PointLight.h"
#include "Texture.h"

RayTracer::RayTracer(unsigned int width, unsigned int height, Scene* scene) 
{
    mRenderer = new Renderer(width, height);
    mShader = new Shader(&mRenderer->mDevice, &mRenderer->mDeviceContext);
    mShader->LoadCS("RayTracer.hlsl");

    // Buffers.
    mShader->CreateConstantBuffer<ConstData>(&ConstData(width, height), &mConstBuffer);
    mShader->CreateCPUwriteGPUreadStructuredBuffer<Vertex>(mMaxNumVertices, &mVertexBuffer);
    mShader->CreateCPUwriteGPUreadStructuredBuffer<PointLight>(mMaxNumPointLights, &mPointLightBuffer);
    mShader->CreateCPUwriteGPUreadStructuredBuffer<PointLight>(mMaxNumDirectionalLights, &mDirectionalLightBuffer);
    mShader->CreateCPUwriteGPUreadStructuredBuffer<Sphere>(mMaxNumSpheres, &mSphereBuffer);
    mShader->CreateCPUwriteGPUreadStructuredBuffer<MetaData>(1, &mMetaBuffer);

    // Texture array buffer.
    ID3D11ShaderResourceView* textureArrayBuffer;
    {
        std::vector<Texture*> textures;
        textures.resize(scene->mTextureIDMap.size());
        for (auto& it : scene->mTextureIDMap) {
            Texture* tex = textures[it.second] = new Texture(&mRenderer->mDevice, &mRenderer->mDeviceContext);
            tex->Load(it.first);
        }
        CreateTexture2DArray(textures, &textureArrayBuffer);
        for (Texture* tex : textures)
            delete tex;
    }

    // Bind.
    mSRVs.push_back(mVertexBuffer);
    mSRVs.push_back(mPointLightBuffer);
    mSRVs.push_back(mDirectionalLightBuffer);
    mSRVs.push_back(mSphereBuffer);
    mSRVs.push_back(mMetaBuffer);
    mSRVs.push_back(textureArrayBuffer);
    mShader->BindCS("RayTracer", &mRenderer->mBackBufferUAV, 1, mSRVs.data(), mSRVs.size(), mConstBuffer);

    textureArrayBuffer->Release();
}

RayTracer::~RayTracer() 
{
    mShader->UnbindCS(1, mSRVs.size());
    mVertexBuffer->Release();
    mPointLightBuffer->Release();
    mDirectionalLightBuffer->Release();
    mSphereBuffer->Release();
    mMetaBuffer->Release();
    mConstBuffer->Release();
    delete mRenderer;
    delete mShader;
}

bool RayTracer::Running() {
    return mRenderer->Running();
}

void RayTracer::Update(Scene& scene) 
{
    // Clear vectors.
    mVertices.clear();

    glm::mat4 viewMatrix = scene.mCamera.mViewMatrix;
    for (std::size_t e = 0; e < scene.mEntites.size(); ++e)
    {
        Entity* entity = scene.mEntites[e];
        Model& model = entity->mModel;
        {   // Transform vertex to view space.
            glm::mat4 modelMatrix = entity->CalculateModelMatrix();
            std::size_t offset = mVertices.size();
            std::size_t numVertices = model.mIndices.size();
            mVertices.resize(mVertices.size() + numVertices);
            for (std::size_t i = 0; i < numVertices; ++i) {
                unsigned int index = model.mIndices[i];
                const Geometry::SkinVertex& skinVertex = model.mVertices[index];
                Vertex& vertex = mVertices[i + offset];
                vertex.pos = glm::vec3(viewMatrix * (modelMatrix * glm::vec4(skinVertex.position, 1.f)));
                vertex.norm = glm::normalize(glm::vec3(viewMatrix * glm::vec4(skinVertex.normal, 0.f)));
                vertex.tang = glm::normalize(glm::vec3(viewMatrix * glm::vec4(skinVertex.tangent, 0.f)));
                vertex.uv = skinVertex.textureCoordinate;
                vertex.diffTexID = entity->mDiffuseTextureID;
                vertex.normTexID = entity->mNormalTextureID;
            }
        }
    }

    // Transform point lights to view space.
    mPointLights.resize(scene.mPointLights.size());
    for (std::size_t i = 0; i < scene.mPointLights.size(); ++i)
    {
        PointLight& pointLight = mPointLights[i] = *scene.mPointLights[i];
        pointLight.pos = glm::vec3(viewMatrix * glm::vec4(pointLight.pos, 1.f));
    }

    // Transform directional lights to view space.
    mDirectionalLights.resize(scene.mDirectionalLights.size());
    for (std::size_t i = 0; i < scene.mDirectionalLights.size(); ++i)
    {
        DirectionalLight& directionalLight = mDirectionalLights[i] = *scene.mDirectionalLights[i];
        directionalLight.dir = glm::normalize(glm::vec3(viewMatrix * glm::vec4(directionalLight.dir, 0.f)));
    }

    // Transform spheres to view space.
    mSpheres.resize(scene.mSpheres.size());
    for (std::size_t i = 0; i < scene.mSpheres.size(); ++i)
    {
        Sphere& sphere = mSpheres[i] = *scene.mSpheres[i];
        sphere.pos = glm::vec3(viewMatrix * glm::vec4(sphere.pos, 1.f));
    }
}

void RayTracer::Render(Scene& scene) 
{
    // Update buffers.
    assert(mVertices.size() <= mMaxNumVertices);
    assert(mPointLights.size() <= mMaxNumPointLights);
    assert(mDirectionalLights.size() <= mMaxNumDirectionalLights);
    assert(mSpheres.size() <= mMaxNumSpheres);
    mShader->WriteStructuredBuffer<Vertex>(mVertices.data(), mVertices.size(), mVertexBuffer);
    mShader->WriteStructuredBuffer<PointLight>(mPointLights.data(), mPointLights.size(), mPointLightBuffer);
    mShader->WriteStructuredBuffer<DirectionalLight>(mDirectionalLights.data(), mDirectionalLights.size(), mDirectionalLightBuffer);
    mShader->WriteStructuredBuffer<Sphere>(mSpheres.data(), mSpheres.size(), mSphereBuffer);
    mShader->WriteStructuredBuffer<MetaData>(&MetaData(mVertices.size(), mPointLights.size(), mDirectionalLights.size(), mSpheres.size(), mNumBounces, mEnergyCoefficient, mSSAA, mFOV), 1, mMetaBuffer);

    // Run compute shader.
    mShader->ExecuteCS(glm::vec3(mRenderer->mWidth / 32, mRenderer->mHeight / 32, 1));

    // Present to window.
    mRenderer->mSwapChain->Present(0,0);
}

void RayTracer::CreateTexture2DArray(std::vector<Texture*>& textures, ID3D11ShaderResourceView** texture2DArrayBuffer)
{
    std::vector<ID3D11Resource*> resources;
    resources.resize(textures.size());
    for (std::size_t i = 0; i < resources.size(); ++i)
    {
        Texture* tex = textures[i];
        assert(tex->mFormat == DXGI_FORMAT_B8G8R8A8_UNORM); //DXGI_FORMAT_R8G8B8A8_UNORM
        assert(tex->mWidth == 1024 && tex->mHeight == 1024);
        assert(tex->mMIpLevels == 1);
        resources[i] = tex->mRes;
    }
    mShader->CreateTexture2DArray(DXGI_FORMAT_B8G8R8A8_UNORM, 1024, 1024, 1, resources, texture2DArrayBuffer);
}

RayTracer::MetaData::MetaData(int numVertices, int numPointLights, int numDirectionalLights, int numSpheres, int numBounces, float energyCoefficient, int ssaa, float fov)
{
    this->numVertices = numVertices;
    this->numPointLights = numPointLights;
    this->numDirectionalLights = numDirectionalLights;
    this->numSpheres = numSpheres;
    this->numBounces = numBounces;
    this->energyCoefficient = energyCoefficient;
    this->ssaa = ssaa;
    this->fov = fov;
}

RayTracer::ConstData::ConstData(int width, int height)
{
    this->width = width;
    this->height = height;
}
