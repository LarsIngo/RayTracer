#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.inl>
#include <map>
#include <string>
#include <Windows.h>

class Texture;

// Window call back procedure.
static LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

// Renders scene to windows.
class Renderer 
{
    public:
        // Constructor.
        // mWidth Window width in pixels.
        // mHeight Window height in pixels.
        Renderer(unsigned int mWidth, unsigned int mHeight);

        // Destructor.
        ~Renderer();

        // Whether window is running of not.
        bool Running() const;

        // Create texture.
        // path Path to file.
        Texture* CreateTexture(std::wstring path);

        // mWidth Window width in pixels.
        unsigned int mWidth;

        // mHeight Window height in pixels.
        unsigned int mHeight;

        // Window handle.
        HWND mHWND;

        // Window swap chain.
        IDXGISwapChain* mSwapChain;

        // DirectX device.
        ID3D11Device* mDevice;

        // DirectX device context.
        ID3D11DeviceContext* mDeviceContext;

        // Backbuffer. Used to render to window.
        ID3D11UnorderedAccessView* mBackBufferUAV = nullptr;

    private:
        // Initialise window, device and device context.
        void Initialise();

        // Texture map.
        std::map<std::wstring, Texture*> mTextureMap;
};
