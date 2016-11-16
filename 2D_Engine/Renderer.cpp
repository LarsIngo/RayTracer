#include "Renderer.h"
#include "Texture.h"

Renderer::Renderer(unsigned int width, unsigned int height) 
{
    mWidth = width;
    mHeight = height;
    Initialise();
}

Renderer::~Renderer() 
{
    mDevice->Release();
    mDeviceContext->Release();
    mSwapChain->Release();
    mBackBufferUAV->Release();
    for (auto& it : mTextureMap)
        delete it.second;
}

bool Renderer::Running() const 
{
    MSG windowMsg = { 0 };

    while (windowMsg.message != WM_QUIT)
    {
        if (PeekMessage(&windowMsg, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&windowMsg);

            DispatchMessage(&windowMsg);
        }
        else
        {
            // If there are no more messages to handle, run a frame 
            return true;
        }
    }
    return false;
}

Texture* Renderer::CreateTexture(std::wstring path)
{
    const auto& it = mTextureMap.find(path);
    if (it != mTextureMap.end())
        return it->second;
    Texture* texture = new Texture(&mDevice, &mDeviceContext);
    texture->Load(path);
    mTextureMap[path] = texture;
    return texture;
}

bool Renderer::GetKeyPressed(int vKey)
{
    return GetAsyncKeyState(vKey);
}

glm::vec2 Renderer::GetMousePosition()
{
    return mMousePosition;
}

bool Renderer::GetMouseInsideWindow()
{
    // get current mouse position in screen coords
    POINT pos = { 0, 0 };
    if (GetCursorPos(&pos))
    {
        // convert position to client window coords
        if (ScreenToClient(mHWND, &pos))
        {
            // get window's client rect
            RECT rcClient = { 0 };
            GetClientRect(mHWND, &rcClient);

            // if mouse cursor is inside rect
            if (PtInRect(&rcClient, pos)) {
                mMousePosition = glm::vec2(static_cast<float>(pos.x) / mWidth * 2.f - 1.f, -(static_cast<float>(pos.y) / mHeight * 2.f - 1.f));
                return true;
            }
        }
    }
    return false;
}

bool Renderer::GetMouseLeftButtonPressed()
{
    return (GetKeyState(VK_LBUTTON) & 0x80) != 0;
}

void Renderer::Initialise() 
{
    // Register the window class to create.
    HINSTANCE applicationHandle = GetModuleHandle(NULL);
    WNDCLASS windowClass;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = applicationHandle;
    windowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = "WindowClass";

    RegisterClass(&windowClass);

    RECT rect = { 0, 0, mWidth, mHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    mHWND = CreateWindow(
        "WindowClass",
        "Window Title",
        WS_OVERLAPPEDWINDOW,
        10,
        10,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        applicationHandle,
        NULL
    );

    ShowWindow(mHWND, SW_SHOWDEFAULT);
    UpdateWindow(mHWND);

    //We initiate the device, device context and swap chain.
    DXGI_SWAP_CHAIN_DESC scDesc;
    scDesc.BufferDesc.Width = mWidth; 		// Using the window's size avoids weird effects. If 0 the window's client width is used.
    scDesc.BufferDesc.Height = mHeight;		// Using the window's size avoids weird effects. If 0 the window's client height is used.
    scDesc.BufferDesc.RefreshRate.Numerator = 0;	// Screen refresh rate as RationalNumber. Zeroing it out makes DXGI calculate it.
    scDesc.BufferDesc.RefreshRate.Denominator = 0;	// Screen refresh rate as RationalNumber. Zeroing it out makes DXGI calculate it.
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// The most common format. Variations include [...]UNORM_SRGB.
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// The order pixel rows are drawn to the back buffer doesn't matter.
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// Since the back buffer and window sizes matches, scaling doesn't matter.
    scDesc.SampleDesc.Count = 1;												// Disable multisampling.
    scDesc.SampleDesc.Quality = 0;												// Disable multisampling.
    scDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS;						// The back buffer will be rendered to.
    scDesc.BufferCount = 1;							// We only have one back buffer.
    scDesc.OutputWindow = mHWND;			// Must point to the handle for the window used for rendering.
    scDesc.Windowed = true;							// Run in windowed mode. Fullscreen is covered in a later sample.
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// This makes the display driver select the most efficient technique.
    scDesc.Flags = 0;								// No additional options.

    D3D11CreateDeviceAndSwapChain(
        nullptr,					// Use the default adapter.
        D3D_DRIVER_TYPE_HARDWARE,	// Use the graphics card for rendering. Other options include software emulation.
        NULL,						// NULL since we don't use software emulation.
        D3D11_CREATE_DEVICE_DEBUG,						// No creation flags.
        nullptr,					// Array of feature levels to try using. With null the following are used 11.0, 10.1, 10.0, 9.3, 9.2, 9.1.
        0,							// The array above has 0 elements.
        D3D11_SDK_VERSION,			// Always use this.
        &scDesc,					// Description of the swap chain.
        &mSwapChain,				// [out] The created swap chain.
        &mDevice,					// [out] The created device.
        nullptr,					// [out] The highest supported feature level (from array).
        &mDeviceContext				// [out] The created device context.
    );

    D3D11_VIEWPORT vp;
    vp.Width = (float)mWidth;
    vp.Height = (float)mHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    mDeviceContext->RSSetViewports(1, &vp);

    ID3D11Texture2D* backBuffer;
    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    mDevice->CreateUnorderedAccessView(backBuffer, nullptr, &mBackBufferUAV);
    backBuffer->Release();
}

LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // If a message has not been handled, send it to the default window procedure for handling.
    return DefWindowProc(handle, message, wParam, lParam);
}
