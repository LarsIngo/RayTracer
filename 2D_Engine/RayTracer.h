#pragma once

#include "DirectionalLight.h"
#include "Sphere.h"
#include "PointLight.h"
#include "Vertex.h"
#include <vector>

class Renderer;
class Scene;
class Shader;
class Texture;

struct ID3D11ShaderResourceView;
struct ID3D11Buffer;

// Ray tracer application.
class RayTracer 
{
    public:
        // Constructor.
        // width Application width in pixels.
        // height Application height in pixels.
        // scene Pointer to which scene to render.
        RayTracer(unsigned int width, unsigned int height, Scene* scene);

        // Destructor.
        ~RayTracer();

        // Whether application is running or not.
        bool Running();

        // Update scene.
        // scene Scene to update.
        void Update(Scene& scene);

        // Render scene.
        // scene Scene to render.
        void Render(Scene& scene);

        // Max number of vertices.
        const int mMaxNumVertices = 5000;

        // Max number of point lights.
        const int mMaxNumPointLights = 10;

        // Max number of directional lights.
        const int mMaxNumDirectionalLights = 10;

        // Max number of spheres.
        const int mMaxNumSpheres = 15;

        // Number of bounces.
        int mNumBounces = 1;

        // Energi coefficient.
        float mEnergyCoefficient = 0.85f;

        // Super sampling anti analyzing. Should be multiples of two.
        int mSSAA = 1;

        // Field of view.
        float mFOV = 3.14f / 4.f;

        // Renderer to render window.
        Renderer* mRenderer = nullptr;

    private:
        // Create texture array.
        // textures Vector of textures.
        // texture2DArrayBuffer Texture 2D array buffer.
        void CreateTexture2DArray(std::vector<Texture*>& textures, ID3D11ShaderResourceView** texture2DArrayBuffer);

        // Compute shader helper class. 
        Shader* mShader = nullptr;

        // Vector of vertex points.
        std::vector<Vertex> mVertices;

        // Structured buffer contaning vertex data.
        ID3D11ShaderResourceView* mVertexBuffer;

        // Vector of point lights.
        std::vector<PointLight> mPointLights;

        // Structured buffer contaning point light data.
        ID3D11ShaderResourceView* mPointLightBuffer;

        // Vector of directional lights.
        std::vector<DirectionalLight> mDirectionalLights;

        // Structured buffer contaning directional light data.
        ID3D11ShaderResourceView* mDirectionalLightBuffer;

        // Vector of spheres.
        std::vector<Sphere> mSpheres;

        // Structured buffer contaning sphere data.
        ID3D11ShaderResourceView* mSphereBuffer;

        // Structured buffer contaning metadata.
        ID3D11ShaderResourceView* mMetaBuffer;

        // Vector of shader resource views.
        std::vector<ID3D11ShaderResourceView*> mSRVs;

        // Constant buffer contaning constant data.
        ID3D11Buffer* mConstBuffer;

        // Meta data.
        struct MetaData {
            // Number of vertices.
            int numVertices;
            // Number of point lights.
            int numPointLights;
            // Number of directinal lights.
            int numDirectionalLights;
            // Number of spheres.
            int numSpheres;
            // Number of bounces.
            int numBounces;
            // Energy coefficient.
            float energyCoefficient;
            // Amount of super sampling anti analyzing.
            int ssaa;
            // Field of view.
            float fov;

            // Constructor.
            MetaData(int numVertices, int numPointLights, int numDirectionalLights, int numSpheres, int numBounces, float energyCoefficient, int ssaa, float fov);
        };

        struct ConstData {
            // Frame width in pixels.
            int width;
            // Frame height in pixels.
            int height;
            // Amount of super sampling anti analyzing.
            float pad[2];

            // Constructor.
            ConstData(int width, int height);
        };
};
