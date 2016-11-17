#include "Shader.h"
#include "DxAssert.h"

Shader::Shader(ID3D11Device** device, ID3D11DeviceContext** deviceContext) {
    mpDevice = device;
    mpDeviceContext = deviceContext;
    assert(mpDevice != nullptr && mpDeviceContext != nullptr);
    CreateSampleState(&mSampleState);

}

Shader::~Shader() {
    for (auto& it : mShaderMap)
        it.second.shader->Release();
    mSampleState->Release();
}

void Shader::CreateTexture2DArray(DXGI_FORMAT format, int width, int height, int mipLevels, std::vector<ID3D11Resource*>& texturesResources, ID3D11ShaderResourceView** buffer)
{
    // http://www.gamedev.net/topic/636900-how-to-create-a-texture2darray-from-files-in-dx11/

    // Create the texture array. Each element in the texture array has the same format/dimensions.
    ID3D11Texture2D* localTextureArray;
    D3D11_TEXTURE2D_DESC texArrDesc;
    ZeroMemory(&texArrDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texArrDesc.Format = format;
    texArrDesc.Width = width;
    texArrDesc.Height = height;
    texArrDesc.ArraySize = texturesResources.size();
    texArrDesc.MipLevels = mipLevels;
    texArrDesc.SampleDesc.Count = 1;
    texArrDesc.SampleDesc.Quality = 0;
    texArrDesc.Usage = D3D11_USAGE_DEFAULT; // D3D11_USAGE_IMMUTABLE
    texArrDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ; // D3D11_CPU_ACCESS_READ
    texArrDesc.MiscFlags = 0;

    DxAssert((*mpDevice)->CreateTexture2D(&texArrDesc, NULL, &localTextureArray), S_OK);

    // Copy individual texture element into texture array.
    for (std::size_t i = 0; i < texturesResources.size(); ++i)
    {
        for (std::size_t j = 0; j < mipLevels; ++j)
        {
            (*mpDeviceContext)->CopySubresourceRegion(localTextureArray, D3D11CalcSubresource(j, i, mipLevels), 0, 0, 0, texturesResources[i], 0, 0);

            //D3D11_MAPPED_SUBRESOURCE mappedResource;
            //ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
            //DxAssert((*mpDeviceContext)->Map(texturesResources[i], j, D3D11_MAP_READ, 0, NULL), S_OK);
            //(*mpDeviceContext)->UpdateSubresource(localTextureArray, D3D11CalcSubresource(j, i, mipLevels), 0, mappedResource.pData, mappedResource.RowPitch, 0);
            //(*mpDeviceContext)->Unmap(texturesResources[i], j);
        }

    }

    // Create a resource view to the texture array.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.ArraySize = texturesResources.size();
    srvDesc.Texture2DArray.FirstArraySlice = 0;
    srvDesc.Texture2DArray.MipLevels = mipLevels;
    srvDesc.Texture2DArray.MostDetailedMip = 0;

    DxAssert((*mpDevice)->CreateShaderResourceView(localTextureArray, &srvDesc, buffer), S_OK);

    localTextureArray->Release();

    //// Create a continues array of textures.
    //std::vector<ID3D11Texture2D*> textureArr;
    //textureArr.resize(textures.size());
    //for (std::size_t i = 0; i < textures.size(); ++i)
    //{
    //    (*mpDeviceContext)->CopyResource(textureArr[i], *textures[i]);
    //}

    //ID3D11Texture2D* localTextureArray;
    //D3D11_TEXTURE2D_DESC texArrDesc;
    //ZeroMemory(&texArrDesc, sizeof(D3D11_TEXTURE2D_DESC));
    //texArrDesc.Format = format;
    //texArrDesc.Width = width;
    //texArrDesc.Height = height;
    //texArrDesc.ArraySize = textures.size();
    //texArrDesc.MipLevels = 1;
    //texArrDesc.SampleDesc.Count = 1;
    //texArrDesc.SampleDesc.Quality = 0;
    //texArrDesc.Usage = D3D11_USAGE_IMMUTABLE;
    //texArrDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    //texArrDesc.CPUAccessFlags = 0;
    //texArrDesc.MiscFlags = 0;

    //D3D11_SUBRESOURCE_DATA* initData = new D3D11_SUBRESOURCE_DATA[textures.size()];
    //for (std::size_t i = 0; i < textures.size(); ++i)
    //{
    //    ZeroMemory(&initData[i], sizeof(D3D11_SUBRESOURCE_DATA));
    //    initData[i].pSysMem = textures.data();
    //    initData[i].SysMemPitch = width * 4; //R8G8B8A8
    //}

    //DxAssert((*mpDevice)->CreateTexture2D(&texArrDesc, initData, &localTextureArray), S_OK);
    //delete[] initData;

    //D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    //ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    //srvDesc.Format = format;
    //srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    //srvDesc.Texture2DArray.ArraySize = textures.size();
    //srvDesc.Texture2DArray.FirstArraySlice = 0;
    //srvDesc.Texture2DArray.MipLevels = 1;
    //srvDesc.Texture2DArray.MostDetailedMip = 0;

    //DxAssert((*mpDevice)->CreateShaderResourceView(localTextureArray, &srvDesc, buffer), S_OK);

    //localTextureArray->Release();
}

void Shader::CreateTexture2D(DXGI_FORMAT format, ID3D11Texture2D* texture, ID3D11ShaderResourceView** buffer)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    DxAssert((*mpDevice)->CreateShaderResourceView(texture, &srvDesc, buffer), S_OK);
}

void Shader::LoadCS(std::string fileName)
{
    cShader shader;

	shader.name = fileName.substr(0, fileName.length() - 5);

	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* shaderBlob = nullptr;
	std::wstring wStr = std::wstring(fileName.begin(), fileName.end());
	HRESULT hr = D3DCompileFromFile(wStr.c_str(), nullptr, nullptr, "CS_main", "cs_5_0", 0, NULL, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		std::string errorMsg = (char*)errorBlob->GetBufferPointer();
		OutputDebugStringA(errorMsg.c_str());
	}
	hr = (*mpDevice)->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &shader.shader);
	shaderBlob->Release();

    mShaderMap[shader.name] = shader;
}

void Shader::BindCS(std::string name, ID3D11UnorderedAccessView** uavs, int numOfUAVs, ID3D11ShaderResourceView** srvs, int numOfSRVs, ID3D11Buffer* cBuffer)
{
    const auto& it = mShaderMap.find(name);
    if (it != mShaderMap.end()) {
        ID3D11ComputeShader* shader = it->second.shader;
        (*mpDeviceContext)->CSSetShader(shader, nullptr, NULL);
        (*mpDeviceContext)->CSSetUnorderedAccessViews(0, numOfUAVs, uavs, nullptr);
        (*mpDeviceContext)->CSSetShaderResources(0, numOfSRVs, srvs);
        if (cBuffer != nullptr)
            (*mpDeviceContext)->CSSetConstantBuffers(0, 1, &cBuffer);
        (*mpDeviceContext)->CSSetSamplers(0, 1, &mSampleState);
    }
}

void Shader::UnbindCS(int numOfUAVs, int numOfSRVs)
{
    (*mpDeviceContext)->CSSetShader(nullptr, nullptr, NULL);
    ID3D11Buffer* const pBuff = nullptr;
    (*mpDeviceContext)->CSSetConstantBuffers(0, 1, &pBuff);
    ID3D11UnorderedAccessView* const pUAV = nullptr;
    for (int i = 0; i < numOfUAVs; ++i)
        (*mpDeviceContext)->CSSetUnorderedAccessViews(i, 1, &pUAV, nullptr);
    ID3D11ShaderResourceView* const pSRV = nullptr;
    for (int i = 0; i < numOfSRVs; ++i)
        (*mpDeviceContext)->CSSetShaderResources(i, 1, &pSRV);
    ID3D11SamplerState* const pSamp = nullptr;
    (*mpDeviceContext)->CSSetSamplers(0, 1, &pSamp);
}

void Shader::ExecuteCS(glm::vec3 dispatchDim)
{
    (*mpDeviceContext)->Dispatch(dispatchDim.x, dispatchDim.y, dispatchDim.z);
}

void Shader::CreateSampleState(ID3D11SamplerState** sampleState)
{
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.MinLOD = -FLT_MAX;
    sampDesc.MaxLOD = FLT_MAX;
    sampDesc.MipLODBias = 0.f;
    sampDesc.MaxAnisotropy = 1;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    DxAssert((*mpDevice)->CreateSamplerState(&sampDesc, sampleState), S_OK);
}
