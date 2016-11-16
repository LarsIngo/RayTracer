#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.inl>
#include <glm/vec2.hpp>
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

        // Get key status.
        // vKey Windows virtual key.
        // Returns whether key is pressed or not.
        bool GetKeyPressed(int vKey);

        // Get mouse inside window.
        // Return whether mouse is inside window or not.
        bool GetMouseInsideWindow();

        // Get mouse postion in screen space.
        // Return mouse position relative window upper left coner.
        glm::vec2 GetMousePosition();

        // Get mouse left button status.
        // Return whether mouse left button is pressed or not.
        bool GetMouseLeftButtonPressed();

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

        // Mouse position.
        glm::vec2 mMousePosition;
};
