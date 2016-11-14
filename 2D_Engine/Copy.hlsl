RWTexture2D<float4> input : register(u0);
RWTexture2D<float4> output : register(u1);

[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{

	output[threadID.xy] = input[threadID.xy];

}