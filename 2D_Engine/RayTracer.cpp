#include "RayTracer.h"

#include <d3d11.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Entity.h"
#include "EntityEntry.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "SkinVertex.h"
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
    mShader->CreateCPUwriteGPUreadStructuredBuffer<EntityEntry>(mMaxNumOfEntities, &mEntityEntryBuffer);
    mShader->CreateCPUwriteGPUreadStructuredBuffer<MetaData>(1, &mMetaBuffer);

    // Texture array.
    ID3D11ShaderResourceView* diffuseTexArray64x64Buffer;
    std::vector<ID3D11Resource*> texturesResources;
    std::vector<Texture*> textures;
    texturesResources.resize(scene->mEntites.size());
    textures.resize(scene->mEntites.size());
    for (std::size_t i = 0; i < texturesResources.size(); ++i)
    {
        Texture* tex = textures[i] = new Texture(&mRenderer->mDevice, &mRenderer->mDeviceContext);
        tex->Load(scene->mEntites[i]->mDiffuseTexPath);
        assert(tex->mFormat == DXGI_FORMAT_R8G8B8A8_UNORM);
        assert(tex->mWidth == 64 && tex->mHeight == 64);
        assert(tex->mMIpLevels == 1);
        texturesResources[i] = tex->mRes;
    }
    mShader->CreateTexture2DArray(DXGI_FORMAT_R8G8B8A8_UNORM, 64, 64, 1, texturesResources, &diffuseTexArray64x64Buffer);

    // Default texture.
    Texture* defaultDiffuseTex = new Texture(&mRenderer->mDevice, &mRenderer->mDeviceContext);
    defaultDiffuseTex->Load(L"assets/DefaultDiffuse.png");

    // Bind.
    mSRVs.push_back(mVertexBuffer);
    mSRVs.push_back(mPointLightBuffer);
    mSRVs.push_back(mEntityEntryBuffer);
    mSRVs.push_back(mMetaBuffer);
    mSRVs.push_back(defaultDiffuseTex->mSRV);
    mSRVs.push_back(diffuseTexArray64x64Buffer);
    mShader->BindCS("RayTracer", &mRenderer->mBackBufferUAV, 1, mSRVs.data(), mSRVs.size(), mConstBuffer);

    delete defaultDiffuseTex;
    diffuseTexArray64x64Buffer->Release();
    for (Texture* tex : textures)
        delete tex;
}

RayTracer::~RayTracer() 
{
    mShader->UnbindCS(1, mSRVs.size());
    mVertexBuffer->Release();
    mPointLightBuffer->Release();
    mEntityEntryBuffer->Release();
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
    mEntityEntries.clear();
    mEntityEntries.resize(scene.mEntites.size());

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
            mEntityEntries[e] = EntityEntry(offset, numVertices);
            for (std::size_t i = 0; i < numVertices; ++i) {
                unsigned int index = model.mIndices[i];
                const Geometry::SkinVertex& skinVertex = model.mVertices[index];
                Vertex& vertex = mVertices[i + offset];
                vertex.pos = glm::vec3(viewMatrix * (modelMatrix * glm::vec4(skinVertex.position, 1.f)));
                vertex.norm = glm::vec3(viewMatrix * (modelMatrix * glm::vec4(skinVertex.normal, 0.f)));
                vertex.uv = skinVertex.textureCoordinate;
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
}

void RayTracer::Render(Scene& scene) 
{
    // Update buffers.
    assert(mVertices.size() <= mMaxNumVertices);
    assert(mPointLights.size() <= mMaxNumPointLights);
    assert(scene.mEntites.size() <= mMaxNumOfEntities);
    mShader->WriteStructuredBuffer<Vertex>(mVertices.data(), mVertices.size(), mVertexBuffer);
    mShader->WriteStructuredBuffer<PointLight>(mPointLights.data(), mPointLights.size(), mPointLightBuffer);
    mShader->WriteStructuredBuffer<EntityEntry>(mEntityEntries.data(), mEntityEntries.size(), mEntityEntryBuffer);
    mShader->WriteStructuredBuffer<MetaData>(&MetaData(mVertices.size(), mPointLights.size(), mNumBounces, scene.mEntites.size()), 1, mMetaBuffer);

    // Run compute shader.
    mShader->ExecuteCS(glm::vec3(mRenderer->mWidth / 32, mRenderer->mHeight / 32, 1));

    // Present to window.
    mRenderer->mSwapChain->Present(0,0);
}

RayTracer::MetaData::MetaData(int numOfVertices, int numOfPointLights, int numBounces, int numEntities)
{
    this->numOfVertices = numOfVertices;
    this->numOfPointLights = numOfPointLights;
    this->numBounces = numBounces;
    this->numEntities = numEntities;
}

RayTracer::ConstData::ConstData(int width, int height)
{
    this->width = width;
    this->height = height;
}
