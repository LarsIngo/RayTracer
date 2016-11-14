#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <ctime>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Draw.h"
#include "Shader.h"

using namespace DirectX;

bool Running();

void Initialise(HWND* mHWND, IDXGISwapChain** mSwapChain, ID3D11UnorderedAccessView** mBackBufferUAV);
LRESULT CALLBACK WindowProcedure(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

DirectX::XMFLOAT2 WASDinput(float speed);

ID3D11Device* mDevice;
ID3D11DeviceContext* mDeviceContext;

const int resX = 1000;
const int resY = 1000;

void Metaball() {

	HWND mHWND;
	IDXGISwapChain* mSwapChain;

	ID3D11UnorderedAccessView* mBackBufferUAV;

	//Initialise the DirectX stuff
	Initialise(&mHWND, &mSwapChain, &mBackBufferUAV);

	Draw draw;

	ID3D11UnorderedAccessView* UAV = draw.CreateUAV(1000, 1000, DXGI_FORMAT_R32_FLOAT);
	XMINT2 ballPos = XMINT2(500, 500);

	XMINT2 a[6];

	float speed = 1;

	while (Running()) {

		draw.Clear(&mBackBufferUAV);
		draw.Clear(&UAV);

		if (GetAsyncKeyState('W')) {

			ballPos = XMINT2(ballPos.x, ballPos.y - speed);

		}
		if (GetAsyncKeyState('A')) {

			ballPos = XMINT2(ballPos.x - speed, ballPos.y);

		}
		if (GetAsyncKeyState('S')) {

			ballPos = XMINT2(ballPos.x, ballPos.y + speed);

		}
		if (GetAsyncKeyState('D')) {

			ballPos = XMINT2(ballPos.x + speed, ballPos.y);

		}

		a[0] = XMINT2(200, 800);
		a[1] = XMINT2(800, 800);
		a[2] = XMINT2(200, 200);
		a[3] = XMINT2(800, 200);
		a[4] = XMINT2(500, 500);
		a[5] = ballPos;

		draw.Metaball(&UAV, a, 6);
		draw.Visualise(&mBackBufferUAV, &UAV);

		mSwapChain->Present(0, 0);

		Sleep(0.1);

	}

}
void Platformer() {

	HWND mHWND;
	IDXGISwapChain* mSwapChain;

	ID3D11UnorderedAccessView* mBackBufferUAV;

	//Initialise the DirectX stuff
	Initialise(&mHWND, &mSwapChain, &mBackBufferUAV);

	Draw draw;

	Rect floor;
	floor.corner = XMFLOAT2(0, 950);
	floor.height = 50;
	floor.width = 1000;

	Rect platform;
	platform.corner = XMFLOAT2(350, 850);
	platform.width = 300;
	platform.height = 50;

	Rect platform2;
	platform2.corner = XMFLOAT2(200, 600);
	platform2.width = 600;
	platform2.height = 15;

	Rect platform3;
	platform3.corner = XMFLOAT2(500, 700);
	platform3.width = 300;
	platform3.height = 15;

	Rect player;
	player.corner = XMFLOAT2(500, 300);
	player.height = 50;
	player.width = 50;

	Rect debug;
	debug.corner = XMFLOAT2(100, 100);
	debug.width = 10;
	debug.height = 10;

	float yVelocity = 0;

	Rect rects[200];

	srand(time(NULL));

	for (int x = 0; x < 200; x++) {

		rects[x].corner = DirectX::XMFLOAT2(50 + rand() % 900, 10 + rand() % 900);
		rects[x].width = 100;
		rects[x].height = 1;

	}

	ID3D11UnorderedAccessView* UAV = draw.CreateUAV(1000, 1000, DXGI_FORMAT_R8G8B8A8_UNORM);

	for (int x = 0; x < 100; x++) {

		draw.DrawRect(&UAV, rects[x], DirectX::XMFLOAT4(0, 0, 1, 1));

	}

	draw.DrawRect(&UAV, floor, XMFLOAT4(0, 1, 0, 1));

	int frames = 0;

	clock_t startTime = clock();

	while (Running()) {

		frames++;

		yVelocity += 0.3;

		if (GetAsyncKeyState('W') && draw.MoveCollide(&UAV, player, XMFLOAT2(0, 1), 1) != -1) {

			yVelocity = -8;

		}

		XMFLOAT2 oldPos = player.corner;
		player.Move(yVelocity, XMFLOAT2(0, 1));
		int collide = draw.Collide(&UAV, player);

		if (collide != 0) {

			player.corner = oldPos;
			int distance = draw.MoveCollide(&UAV, player, XMFLOAT2(0, 1), yVelocity);
			player.Move(distance, XMFLOAT2(0, 1), false);

			yVelocity = 0;

		}

		XMFLOAT2 input = WASDinput(6);

		oldPos = player.corner;
		player.Move(input.x, XMFLOAT2(1, 0));
		collide = draw.Collide(&UAV, player);

		if (collide != 0) {

			player.corner = oldPos;

			int distance = draw.MoveCollide(&UAV, player, XMFLOAT2(1, 0), input.x * 2);
			player.Move(distance, XMFLOAT2(1, 0), false);

		}

		draw.Copy(&UAV, &mBackBufferUAV);
		draw.DrawRect(&mBackBufferUAV, player, XMFLOAT4(1, 1, 1, 1));
		mSwapChain->Present(0, 0);

		draw.Clear(&mBackBufferUAV);


		clock_t endTime = clock();
		clock_t clockTicksTaken = endTime - startTime;
		double timeInSeconds = clockTicksTaken / (double)CLOCKS_PER_SEC;

		int b = std::time(NULL);
		float a = frames / timeInSeconds;
		printf("%f\n", a);
		Sleep(10);

	}

}


//void main() {
//
//	Metaball();
//
//}

/*
void mainSpinThing() {

	HWND mHWND;
	IDXGISwapChain* mSwapChain;

	ID3D11UnorderedAccessView* mBackBufferUAV;

	//Initialise the DirectX stuff
	Initialise(&mHWND, &mSwapChain, &mBackBufferUAV);

	Draw draw;

	ID3D11UnorderedAccessView* UAV = draw.CreateUAV(1000, 1000, DXGI_FORMAT_R8G8B8A8_UNORM);

	float angle = 0;
	float angle2 = 0;

	Rect rect;
	rect.center = XMFLOAT2(500, 500);
	rect.width = 10;
	rect.height = 10;

	Rect rect2;
	rect2.center = XMFLOAT2(700, 500);
	rect2.width = 100;
	rect2.height = 1000;

	Rect rect3;
	rect3.center = XMFLOAT2(500, 700);
	rect3.width = 10;
	rect3.height = 10;

	while (Running()) {

		//draw.MoveRect(&mBackBufferUAV, rect3, XMFLOAT4(1, 1, 0, 0), XMFLOAT2(cosf(angle + 1.5f), sinf(angle + 1.5f)), 170);
		rect3.center = XMFLOAT2(700, 500);
		rect3.width = 10;
		rect3.height = 10;
		draw.MoveRect(&mBackBufferUAV, rect3, XMFLOAT4(1, 0, 1, 0), XMFLOAT2(cosf(angle + 1.5f), sinf(angle + 1.5f)), 170);
		rect3.center = XMFLOAT2(500, 300);
		rect3.width = 10;
		rect3.height = 10;
		draw.MoveRect(&mBackBufferUAV, rect3, XMFLOAT4(1, 1, 1, 0), XMFLOAT2(cosf(angle + 1.5f), sinf(angle + 1.5f)), 170);
		rect3.center = XMFLOAT2(300, 500);
		rect3.width = 10;
		rect3.height = 10;
		draw.MoveRect(&mBackBufferUAV, rect3, XMFLOAT4(1, 1, 0, 0), XMFLOAT2(cosf(angle + 1.5f), sinf(angle + 1.5f)), 170);
		rect3.center = XMFLOAT2(500, 700);
		rect3.width = 10;
		rect3.height = 10;
		draw.MoveRect(&mBackBufferUAV, rect3, XMFLOAT4(1, 0, 0, 0), XMFLOAT2(cosf(angle + 1.5f), sinf(angle + 1.5f)), 170);

		int a = draw.MoveCollide(&mBackBufferUAV, rect, XMFLOAT2(cosf(angle2), sinf(angle2)), 300);

		draw.MoveRect(&mBackBufferUAV, rect, XMFLOAT4(0, 0, 1, 0), XMFLOAT2(cosf(angle2), sinf(angle2)), 10);

		if (a != -1) {

			XMFLOAT2 oldCent = rect.center;
			rect.Move(a, XMFLOAT2(cosf(angle2), sinf(angle2)));
			draw.DrawRect(&mBackBufferUAV, rect, XMFLOAT4(1, 0, 0, 0));

			rect.center = oldCent;

		}

		mSwapChain->Present(0, 0);

		printf("%d\n", a);

		draw.Clear(&mBackBufferUAV);

		angle += 0.01f;
		angle2 -= 0.02f;

		Sleep(10);

	}

}

void mainSimplePlatformer() {

	HWND mHWND;
	IDXGISwapChain* mSwapChain;

	ID3D11UnorderedAccessView* mBackBufferUAV;

	//Initialise the DirectX stuff
	Initialise(&mHWND, &mSwapChain, &mBackBufferUAV);

	Draw draw;

	ID3D11UnorderedAccessView* UAV = draw.CreateUAV(1000, 1000, DXGI_FORMAT_R8G8B8A8_UNORM);

	Rect rect;
	rect.center = DirectX::XMFLOAT2(500, 0);
	rect.width = 10;
	rect.height = 10;

	Rect rect2;
	rect2.center = DirectX::XMFLOAT2(500, 1000);
	rect2.width = 1000;
	rect2.height = 100;

	Rect rect3;
	rect3.center = DirectX::XMFLOAT2(500, 500);
	rect3.width = 300;
	rect3.height = 100;

	draw.DrawRect(&UAV, rect, DirectX::XMFLOAT4(1, 0, 0, 0));

	float speed = 3;

	float yVelocity = 0;

	Rect rects[100];

	srand(time(NULL));

	for (int x = 0; x < 100; x++) {

		rects[x].center = DirectX::XMFLOAT2(50 + rand() % 900, 10 + rand() % 900);
		rects[x].width = 50;
		rects[x].height = 10;

	}

	while (true) {

		draw.Clear(&UAV);
		draw.Clear(&mBackBufferUAV);

		for (int x = 0; x < 100; x++) {
			
			draw.DrawRect(&UAV, rects[x], DirectX::XMFLOAT4(1, 0, 0, 0));

		}

		draw.DrawRect(&UAV, rect2, DirectX::XMFLOAT4(1, 0, 0, 0));

		DirectX::XMFLOAT2 oldCenter = rect.center;
		DirectX::XMFLOAT2 input = WASDinput(2);

		if (input.y > 0 && yVelocity == 0) {

			yVelocity += input.y;

		}

		if (input.y > 0) {

			yVelocity += 0.01f;

		}

		yVelocity -= 0.04f;

		rect.Move(DirectX::XMFLOAT2(0, yVelocity));
		int b = draw.Collide(&UAV, rect);

		if (b != 0) {

			rect.center = oldCenter;
			yVelocity = 0;

		}

		input = WASDinput(3);

		rect.Move(DirectX::XMFLOAT2(input.x, 0));
		b = draw.Collide(&UAV, rect);

		if (b != 0) {

			rect.center = DirectX::XMFLOAT2(oldCenter.x, rect.center.y);

		}

		draw.Copy(&UAV, &mBackBufferUAV);
		draw.DrawRect(&mBackBufferUAV, rect, DirectX::XMFLOAT4(1, 1, 1, 1));
		mSwapChain->Present(0, 0);
			
		Sleep(2);

	}
	
}

*/

DirectX::XMFLOAT2 WASDinput(float speed) {

	DirectX::XMFLOAT2 input = DirectX::XMFLOAT2(0, 0);

	if (GetAsyncKeyState('W'))
	{
		input = DirectX::XMFLOAT2(input.x, input.y + speed);
	}
	if (GetAsyncKeyState('A'))
	{
		input = DirectX::XMFLOAT2(input.x - speed, input.y);
	}
	if (GetAsyncKeyState('S'))
	{
		input = DirectX::XMFLOAT2(input.x, input.y - speed);
	}
	if (GetAsyncKeyState('D'))
	{
		input = DirectX::XMFLOAT2(input.x + speed, input.y);
	}

	return input;

}

bool Running()
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
			// Usually Update() + Render() but in this sample only the latter is needed.
			return true;
		}
	}
	return false;
}

void Initialise(HWND* mHWND, IDXGISwapChain** mSwapChain, ID3D11UnorderedAccessView** mBackBufferUAV)
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

	*mHWND = CreateWindow(
		"WindowClass",
		"Window Title",
		WS_OVERLAPPEDWINDOW,
		10,
		10,
		resX,
		resY,
		NULL,
		NULL,
		applicationHandle,
		NULL
		);

	ShowWindow(*mHWND, SW_SHOWDEFAULT);
	UpdateWindow(*mHWND);

	//We initiate the device, device context and swap chain.
	DXGI_SWAP_CHAIN_DESC scDesc;
	scDesc.BufferDesc.Width = resX; 		// Using the window's size avoids weird effects. If 0 the window's client width is used.
	scDesc.BufferDesc.Height = resY;		// Using the window's size avoids weird effects. If 0 the window's client height is used.
	scDesc.BufferDesc.RefreshRate.Numerator = 0;	// Screen refresh rate as RationalNumber. Zeroing it out makes DXGI calculate it.
	scDesc.BufferDesc.RefreshRate.Denominator = 0;	// Screen refresh rate as RationalNumber. Zeroing it out makes DXGI calculate it.
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// The most common format. Variations include [...]UNORM_SRGB.
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// The order pixel rows are drawn to the back buffer doesn't matter.
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// Since the back buffer and window sizes matches, scaling doesn't matter.
	scDesc.SampleDesc.Count = 1;												// Disable multisampling.
	scDesc.SampleDesc.Quality = 0;												// Disable multisampling.
	scDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS;						// The back buffer will be rendered to.
	scDesc.BufferCount = 1;							// We only have one back buffer.
	scDesc.OutputWindow = *mHWND;			// Must point to the handle for the window used for rendering.
	scDesc.Windowed = true;							// Run in windowed mode. Fullscreen is covered in a later sample.
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// This makes the display driver select the most efficient technique.
	scDesc.Flags = 0;								// No additional options.

	D3D11CreateDeviceAndSwapChain(
		nullptr,					// Use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,	// Use the graphics card for rendering. Other options include software emulation.
		NULL,						// NULL since we don't use software emulation.
		NULL,						// No creation flags.
		nullptr,					// Array of feature levels to try using. With null the following are used 11.0, 10.1, 10.0, 9.3, 9.2, 9.1.
		0,							// The array above has 0 elements.
		D3D11_SDK_VERSION,			// Always use this.
		&scDesc,					// Description of the swap chain.
		mSwapChain,				// [out] The created swap chain.
		&mDevice,					// [out] The created device.
		nullptr,					// [out] The highest supported feature level (from array).
		&mDeviceContext				// [out] The created device context.
		);

	D3D11_VIEWPORT vp;
	vp.Width = (float)resX;
	vp.Height = (float)resY;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	mDeviceContext->RSSetViewports(1, &vp);

	ID3D11Texture2D* backBuffer;
	(*mSwapChain)->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	mDevice->CreateUnorderedAccessView(backBuffer, nullptr, mBackBufferUAV);
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