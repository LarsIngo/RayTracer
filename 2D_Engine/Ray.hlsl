RWTexture2D<float4> output : register(u0);

cbuffer CONST_BUFFER : register(b0)
{

	float2 origin;
	float2 direction;
	float4 color;

	float length;

}


[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{

	float dirLength = sqrt(direction.x * direction.x + direction.y * direction.y);
	float2 normDir = float2(direction.x / dirLength, direction.y / dirLength);
	
	if (threadID.x == origin.x && threadID.y == origin.y) {

		int x = 0;
		int y = 0;

		int i = 0;
		do {
		
			x = threadID.x + i * normDir.x;
			y = threadID.y + i * normDir.y;
		
			output[int2(x, y)] = color;

			i++;

		} while (sqrt(abs(threadID.x - x) * abs(threadID.x - x) + abs(threadID.y - y) * abs(threadID.y - y)) < length);

	}

}