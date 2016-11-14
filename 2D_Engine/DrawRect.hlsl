RWTexture2D<float4> output : register(u0);

cbuffer CONST_BUFFER : register(b0)
{

	float2 corner;
	float width;
	float height;

	float4 color;

}

[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{

	if (threadID.x < corner.x + width && threadID.x >= corner.x)
		if (threadID.y < corner.y + height && threadID.y >= corner.y)
			output[threadID.xy] = color;

}