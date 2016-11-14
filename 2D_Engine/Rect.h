#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Rect {

public:
	XMFLOAT2 corner;
	float width;
	float height;

	void Move(XMFLOAT2 xy);
	void Move(int x, int y);
	void Move(float distance, XMFLOAT2 dir, bool close = false);

};