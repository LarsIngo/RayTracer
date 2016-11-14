RWTexture2D<float> output : register(u0);

cbuffer CONST_BUFFER : register(b0)
{

	int ballCount;

}

RWStructuredBuffer<int2> ball : register(u1);

[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{

	output[threadID.xy] = 0;

	for (int i = 0; i < ballCount; i++) {

		int dX = threadID.x - ball[i].x;
		int dY = threadID.y - ball[i].y;

		output[threadID.xy] += 10 / (pow(dX, 2) + pow(dY, 2));//abs(1000.0 / sqrt(pow(abs(ball[i].x - threadID.x), 2) + pow(abs(ball[i].y - threadID.y), 2)));

	}

}