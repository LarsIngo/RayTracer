RWTexture2D<float4> output : register(u0);

cbuffer CONST_BUFFER : register(b0)
{

	//Rectangle
	float2 corner;
	float width;
	float height;

	//Rays
	float2 direction;
	float length;


}

RWStructuredBuffer<int> distance : register(u1);

[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{
	float dirLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	float2 normDir = float2(direction.x / dirLength, direction.y / dirLength);

	bool right = threadID.x == corner.x + width;
	bool top = threadID.y == corner.y - 1;
	bool left = threadID.x == corner.x - 1;
	bool bottom = threadID.y == corner.y + height;

	if (normDir.x == 0) {

		right = false;
		left = false;

	}
	if (normDir.y == 0) {

		top = false;
		bottom = false;

	}

	bool ySpan = threadID.y < corner.y + height && threadID.y >= corner.y;
	bool xSpan = threadID.x < corner.x + width && threadID.x >= corner.x;

	int tmp;

	if (normDir.x >= 0) {

		if (normDir.y >= 0) {

			if ((right && ySpan) || (bottom && xSpan)) {

				int x = 0;
				int y = 0;

				int i = 0;
				do {

					x = ceil(threadID.x + i * normDir.x);
					y = ceil(threadID.y + i * normDir.y);

					if (output[int2(x, y)].w != 0) {

						int dist = sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f));
						InterlockedMin(distance[0], dist, tmp);
						break;

					}

					i++;

				} while (ceil(sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f))) < abs(length));

			}

		}
		else {

			if ((right && ySpan) || (top && xSpan)) {

				int x = 0;
				int y = 0;

				int i = 0;
				do {

					x = ceil(threadID.x + i * normDir.x);
					y = ceil(threadID.y + i * normDir.y);

					if (output[int2(x, y)].w != 0) {

						int dist = sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f));
						InterlockedMin(distance[0], dist, tmp);
						break;

					}

					i++;

				} while (ceil(sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f))) < abs(length));

			}

		}

	}
	else {

		if (normDir.y >= 0) {

			if ((left && ySpan) || (bottom && xSpan)) {

				int x = 0;
				int y = 0;

				int i = 0;
				do {

					x = ceil(threadID.x + i * normDir.x);
					y = ceil(threadID.y + i * normDir.y);

					if (output[int2(x, y)].w != 0) {

						int dist = sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f));
						InterlockedMin(distance[0], dist, tmp);
						break;

					}

					i++;

				} while (ceil(sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f))) < abs(length));

			}

		}
		else {

			if ((left && ySpan) || (top && xSpan)) {

				int x = 0;
				int y = 0;

				int i = 0;
				do {

					x = ceil(threadID.x + i * normDir.x);
					y = ceil(threadID.y + i * normDir.y);

					if (output[int2(x, y)].w != 0) {

						int dist = sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f));
						InterlockedMin(distance[0], dist, tmp);
						break;

					}

					i++;

				} while (ceil(sqrt(pow(i * normDir.x, 2.0f) + pow(i * normDir.y, 2.0f))) < abs(length));

			}

		}

	}

}