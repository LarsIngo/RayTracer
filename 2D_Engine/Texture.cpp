#include "Texture.h"

#include "DxAssert.h"
#include <d3d11.h>
#include <WICTextureLoader.h>

Texture::Texture(ID3D11Device** device, ID3D11DeviceContext** deviceContext)
{
    mpDevice = device;
    mpDeviceContext = deviceContext;
}

Texture::~Texture()
{
    if (mSRV != nullptr)
        mSRV->Release();
    if (mRes != nullptr)
        mRes->Release();
    if (mTexture2D != nullptr)
        mTexture2D->Release();
}

void Texture::Load(std::wstring path)
{
    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    texDesc.MiscFlags = 0;
    DxAssert(DirectX::CreateWICTextureFromFileEx(*mpDevice, path.c_str(), 0, texDesc.Usage, texDesc.BindFlags, texDesc.CPUAccessFlags, texDesc.MiscFlags, DirectX::WIC_LOADER_DEFAULT, &mRes, &mSRV), S_OK);
    mRes->QueryInterface(&mTexture2D);
    D3D11_TEXTURE2D_DESC desc;
    mTexture2D->GetDesc(&desc);
    mWidth = desc.Width;
    mHeight = desc.Height;
    mFormat = desc.Format;
    mMIpLevels = desc.MipLevels;
}
