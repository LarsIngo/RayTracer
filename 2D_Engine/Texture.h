#pragma once

#include <d3d11.h>
#include <string>

// A texture.
class Texture
{
    public:
        // Constructor.
        // device Pointer to directX device.
        // deviceContext Pointer to directX device context.
        Texture(ID3D11Device** device, ID3D11DeviceContext** deviceContext);

        // Destructor.
        ~Texture();

        // Load model.
        // path Path to texture.
        void Load(std::wstring path);

        // Shader resource view.
        ID3D11ShaderResourceView* mSRV = nullptr;

        // Shader resource.
        ID3D11Resource* mRes = nullptr;

        // Texture 2D
        ID3D11Texture2D* mTexture2D = nullptr;

        // Texture width in pixels.
        int mWidth = -1;

        // Texture height in pixels.
        int mHeight = -1;

        // Number mip map levels.
        int mMIpLevels = -1;

        // Texture height in pixels.
        DXGI_FORMAT mFormat;

    private:
        // Pointer to directX device.
        ID3D11Device** mpDevice = nullptr;

        // Pointer to directX device context.
        ID3D11DeviceContext** mpDeviceContext = nullptr;
};
