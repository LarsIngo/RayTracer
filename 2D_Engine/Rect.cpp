#include "Rect.h"

void Rect::Move(DirectX::XMFLOAT2 xy) {

	corner = DirectX::XMFLOAT2(corner.x + xy.x, corner.y - xy.y);

}
void Rect::Move(int x, int y) {

	corner = DirectX::XMFLOAT2(corner.x + x, corner.y - y);

}

void Rect::Move(float distance, XMFLOAT2 dir, bool close) {

	if (distance == 0)
		return;

	float dirLen = sqrt(powf(dir.x, 2) + powf(dir.y, 2));
	XMFLOAT2 dirNorm = XMFLOAT2(dir.x / dirLen, dir.x / dirLen);

	if (distance < 0) {

		if (close)
			distance += 1;

		dirNorm = XMFLOAT2(-dirNorm.x, -dirNorm.y);
		corner = XMFLOAT2(floor(corner.x + dir.x * distance), floor(corner.y + dir.y * distance));

	}
	else {

		if (close)
			distance -= 1;

		corner = XMFLOAT2(ceil(corner.x + dir.x * distance), ceil(corner.y + dir.y * distance));

	}
	

}

