RWTexture2D<float4> output : register(u0);
RWTexture2D<float> input : register(u1);


[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID, uint groupIndex : SV_GroupIndex, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{

	if(input[threadID.xy] > 0.0005f)
		output[threadID.xy] = float4(1, 0, 0, 0);


}