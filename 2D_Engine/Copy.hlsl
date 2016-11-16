RWTexture2D<float4> g_Source : register(u0);
RWTexture2D<float4> g_Target : register(u1);

cbuffer CONST_BUFFER : register(b0)
{
    // Source frame width in pixels.
    const int g_SourceWidth;
    // Source frame height in pixels.
    const int g_SourceWeight;
    // Traget frame width in pixels.
    const int g_TargetWidth;
    // Traget frame height in pixels.
    const int g_TargetWeight;
}


[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID)
{
    g_Target[threadID.xy] = g_Source[threadID.xy];

}