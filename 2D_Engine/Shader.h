#pragma once

#include "DxAssert.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

class Shader {
    public:
        // Constructor.
        // device Pointer to directX device.
        // deviceContext Pointer to directX device context.
        Shader(ID3D11Device** device, ID3D11DeviceContext** deviceContext);

        // Destructor.
        ~Shader();

        // Loads compute shader.
        // fileName Shader name.
	    void LoadCS(std::string fileName);

        // Bind compute shader.
        // name Shader name.
        // uavs Pointer to array of unordered access views.
        // numOfUAVs Number of unordered access views.
        // srvs Pointer to array of shader resource views.
        // numOfSRVs Number of shader resource views.
        // cBuffer Pointer to constant buffer.
        void BindCS(std::string name, ID3D11UnorderedAccessView** uavs = nullptr, int numOfUAVs = 0, ID3D11ShaderResourceView** srvs = nullptr, int numOfSRVs = 0, ID3D11Buffer* cBuffer = nullptr);
        
        // Unbind compute shader.
        // numOfUAVs Number of unordered access views.
        // numOfSRVs Number of shader resource views.
        void UnbindCS(int numOfUAVs, int numOfSRVs);

        // Dispatches compute shader.
        // shader Shader name.
        // dispatchDim Dims to dispatch.
        // uavs Pointer to array of unordered access views.
        // numOfUAVs Number of unordered access views.
        // srvs Pointer to array of shader resource views.
        // numOfSRVs Number of shader resource views.
        // cBuffer Pointer to constant buffer.
	    void ExecuteCS(glm::vec3 dispatchDim);

        // Creates constant buffer.
        // data Pointer to data.
        // cBuffer Constant buffer.
	    template <typename T>
	    void CreateConstantBuffer(T* data, ID3D11Buffer** cBuffer);

        // Creates source stuctured buffer (CPU write, GPU read).
        // numOfElements Maximum number of elements (size of buffer equals sizeOf(T) * numOfElements).
        // buffer Structured buffer.
	    template <typename T>
	    void CreateCPUwriteGPUreadStructuredBuffer(int numOfElements, ID3D11ShaderResourceView** buffer);

        // Creates target stuctured buffer (CPU read, GPU write).
        // numOfElements Maximum number of elements (size of buffer equals sizeOf(T) * numOfElements).
        // buffer Structured buffer.
        template <typename T>
        void CreateCPUreadGPUwriteStructuredBuffer(int numOfElements, ID3D11UnorderedAccessView** buffer);

        // Write stuctured data to the GPU.
        // data Pointer to array of data.
        // numOfElements Number of elements in array.
        // buffer Structured buffer.
        template <typename T>
        void WriteStructuredBuffer(T* data, int numOfElements, ID3D11ShaderResourceView* buffer);

        // Reads structured data from the GPU.
        // buffer Structured buffer.
        // Returns pointer to data.
	    template <typename T>
	    T* ReadStructuredBuffer(ID3D11UnorderedAccessView* buffer);

        // Creates texture 2D array.
        // format Texture format.
        // width Texture width in pixels.
        // mipLevels Number of mip map levels.
        // height Texture height in pixels.
        // texturesResources Vector of textures resources.
        // buffer Texture 2D array buffer.
        void CreateTexture2DArray(DXGI_FORMAT format, int width, int height, int mipLevels, std::vector<ID3D11Resource*>& texturesResources, ID3D11ShaderResourceView** buffer);

        // Creates texture 2D.
        // format Texture format.
        // texture Texture 2D.
        // buffer Texture 2D buffer.
        void CreateTexture2D(DXGI_FORMAT format, ID3D11Texture2D* texture, ID3D11ShaderResourceView** buffer);

    private:
        // Compute shader struct.
        struct cShader {

            // Shader name.
            std::string name;

            // Compute shader.
            ID3D11ComputeShader* shader;
        };

        void CreateSampleState(ID3D11SamplerState** sampleState);

        // Map of compute shader.
        std::map<std::string, cShader> mShaderMap;

        // Pointer to directX device.
        ID3D11Device** mpDevice;

        // Pointer to directX device context.
        ID3D11DeviceContext** mpDeviceContext;

        // Texture samples
        ID3D11SamplerState* mSampleState;
};

template <typename T>
inline void Shader::CreateConstantBuffer(T* data, ID3D11Buffer** cBuffer)
{
	D3D11_BUFFER_DESC cbDesc;
    ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));
	cbDesc.ByteWidth = sizeof(T);
	cbDesc.Usage = D3D11_USAGE_IMMUTABLE;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
    initData.pSysMem = data;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    DxAssert((*mpDevice)->CreateBuffer(&cbDesc, &initData, cBuffer), S_OK);
}

template <typename T>
inline void Shader::CreateCPUwriteGPUreadStructuredBuffer(int numOfElements, ID3D11ShaderResourceView** buffer)
{
    ID3D11Buffer* localBuffer;
	D3D11_BUFFER_DESC bDesc;
    ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
    bDesc.ByteWidth = sizeof(T) * numOfElements;
    bDesc.Usage = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bDesc.StructureByteStride = sizeof(T);
    DxAssert((*mpDevice)->CreateBuffer(&bDesc, NULL, &localBuffer), S_OK);

    D3D11_SHADER_RESOURCE_VIEW_DESC srcDesc;
    ZeroMemory(&srcDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srcDesc.Format = DXGI_FORMAT_UNKNOWN;
    srcDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srcDesc.Buffer.FirstElement = 0;
    srcDesc.Buffer.ElementOffset = 0;
    srcDesc.Buffer.NumElements = numOfElements;
    DxAssert((*mpDevice)->CreateShaderResourceView(localBuffer, &srcDesc, buffer), S_OK);

    localBuffer->Release();
}

template <typename T>
inline void Shader::CreateCPUreadGPUwriteStructuredBuffer(int numOfElements, ID3D11UnorderedAccessView** buffer)
{
    ID3D11Buffer* uavBuffer;
    D3D11_BUFFER_DESC bDesc;
    ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
    bDesc.ByteWidth = sizeof(T) * numOfElements;
    bDesc.Usage = D3D11_USAGE_STAGING;
    bDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    bDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bDesc.StructureByteStride = sizeof(T);
    DxAssert((*mpDevice)->CreateBuffer(&bDesc, NULL, &uavBuffer), S_OK);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement = 0;
    uavDesc.Buffer.NumElements = numOfElements;
    uavDesc.Buffer.Flags = 0;
    DxAssert((*mpDevice)->CreateUnorderedAccessView(uavBuffer, &uavDesc, buffer), S_OK);

    uavBuffer->Release();
}

template <typename T>
inline void Shader::WriteStructuredBuffer(T* data, int numOfElements, ID3D11ShaderResourceView* buffer)
{
    ID3D11Resource* res;
    buffer->GetResource(&res);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    DxAssert((*mpDeviceContext)->Map(res, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource), S_OK);
    memcpy(mappedResource.pData, data, sizeof(T) * numOfElements);
    (*mpDeviceContext)->Unmap(res, 0);
    res->Release();
}

template <typename T>
inline T* Shader::ReadStructuredBuffer(ID3D11UnorderedAccessView* buffer)
{
	ID3D11Resource* res;
    buffer->GetResource(&res);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    DxAssert((*mpDeviceContext)->Map(res, 0, D3D11_MAP_READ, 0, &mappedResource), S_OK);
    DxAssert((*mpDeviceContext)->Unmap(res, 0), S_OK);
    res->Release();

	return (T*)(mappedResource.pData);
}