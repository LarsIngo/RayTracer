RWTexture2D<float4> output : register(u0);

cbuffer CONST_BUFFER : register(b0)
{

	float2 origin;
	float2 direction;

	float length;

}

RWStructuredBuffer<int> distance : register(u1);
RWStructuredBuffer<int> object : register(u1);

[numthreads(1, 1, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{

	float dirLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	float2 normDir = float2(direction.x / dirLength, direction.y / dirLength);

	int tmp;

	int x = 0;
	int y = 0;

	int i = 0;
	do {

		x = ceil(origin.x + i * normDir.x);
		y = ceil(origin.y + i * normDir.y);

		if (output[int2(x, y)].w != 0) {

			int dist = ceil(sqrt(pow(i * normDir.x, 2) + pow(i * normDir.y, 2)));
			distance[0] = dist;
			object[0] = output[int2(x, y)].w * 255;
			break;

		}

		i++;

	} while (sqrt(pow(i * normDir.x, 2) + pow(i * normDir.y, 2)) < length);


}