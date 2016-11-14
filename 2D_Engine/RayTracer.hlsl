
#define FLT_MAX 3.402823466e+38F;

// Sample state.
SamplerState g_SampAni : register(s0);

// Render target.
RWTexture2D<float4> g_Target : register(u0);

cbuffer CONST_BUFFER : register(b0)
{
    // Screen width in pixels.
    const int g_width;
    // Screen height in pixels.
    const int g_height;
    float2 pad;
}

// Vertex point.
struct Vertex 
{
    // Position.
    float3 pos;
    // Normal.
    float3 norm;
    // Uv.
    float2 uv;
};
// Vertex buffer.
StructuredBuffer<Vertex> g_VertexBuffer : register(t0);

// Point light.
struct PointLight
{
    // Position.
    float3 pos;
    // Color.
    float3 col;
    // Max distance.
    float maxDistance;
    float pad;
};
// Vertex buffer.
StructuredBuffer<PointLight> g_PointLightBuffer : register(t1);

// Entity entries.
struct EntityEntry
{
    // Index to first vertex in vertex buffer.
    int offset;
    // Number of vertices in entity.
    int numOfVertices;
    float2 pad;
};
// Vertex buffer.
StructuredBuffer<EntityEntry> g_EntityEntries : register(t2);

// Meta data.
struct MetaData 
{
    // Number of vertices in vertex buffer.
    int numOfVertices;
    // Number of lights in point light buffer.
    int numOfPointLights;
    // Number of bounces.
    int numBounces;
    // Number of entities.
    int numEntities;
};
// Meta buffer.
StructuredBuffer<MetaData> g_MetaBuffer : register(t3);

// Diffuse texture.
Texture2D<float4> g_DiffuseTexture : register(t4);

// Diffuse texture array.
Texture2DArray<float4> g_DiffuseTextureArray : register(t5);

// Hit data.
struct HitData 
{
    // Scalar to closet ray intersection.
    float t;
    // Offset to first vertex.
    int vertexOffset;
};

// Sample.
struct Sample
{
    // Diffuse.
    float3 diffuse;
    // Dependent on Fresnel effect.
    float diffuseFactor;
};

// Intersect ray vs scene.
// rayOrigin Origin of ray.
// rayDirection Direction of ray.
// Returns hitdata.
HitData RayVsScene(float3 rayOrigin, float3 rayDirection);

// Intersect ray vs triangle.
// rayOrigin Origin of ray.
// rayDirection Direction of ray.
// v0, v1, v2 Vertex positions.
// Returns scalar to closest hit point.
float RayVsTriangle(float3 rayOrigin, float3 rayDirection, float3 v0, float3 v1, float3 v2);

// Interpolates vertex values.
// hitPoint Point of intersection.
// v0, v1, v2 Vertex points to interpolate.
// Returns interpolated vertex.
Vertex Interpolate(float3 hitPoint, Vertex v0, Vertex v1, Vertex v2);

// Calculate sample for vertex.
// hitPoint Point of intersection.
// vertex Interpolated vertex.
// entityID Index of which entity vertex corresponds.
// Returns color.
Sample CalculateSample(float3 hitPoint, Vertex vertex, int entityID);

// Get id of which entity this vertex belongs to.
// vertexIndex Index of vertex.
// Return id.
int GetEntityID(int vertexIndex);

// One thread for each pixel in screen.
[numthreads(32, 32, 1)]
void CS_main(uint3 threadID : SV_DispatchThreadID)
{
    // Contants.
    const float fov = 3.14f / 4.f;
    const float focalDistance = 0.1f;
    const float3 focalPoint = float3(0.f, 0.f, -focalDistance);
    const int width = g_width;
    const int height = g_height;
    const float aspectRatio = float(width) / height;
    const float heightbyWidth = 1.f / aspectRatio;
    const float R = tan(fov / 2.f) * focalDistance;
    const float U = heightbyWidth * R;

    // Ray.
    float3 rayOrigin = float3(-R + 2 * R * threadID.x / width, U - 2 * U * threadID.y / height, 0.f);
    float3 rayDirection = normalize(rayOrigin - focalPoint);

    const int numBounces = g_MetaBuffer[0].numBounces;
    float totalDistance = 0.001f;
    Sample samples[10]; // TODO from constant buffer.
    for (int i = 0; i < numBounces; ++i)
    {
        // Intersect ray vs scene.
        HitData hitData = RayVsScene(rayOrigin, rayDirection);
        int entityID = GetEntityID(hitData.vertexOffset);

        // Interpolate vertex.
        float3 hitPoint = rayOrigin + rayDirection * hitData.t;
        Vertex vertex = Interpolate(hitPoint, g_VertexBuffer[hitData.vertexOffset], g_VertexBuffer[hitData.vertexOffset + 1], g_VertexBuffer[hitData.vertexOffset + 2]);

        // Calculate pixel color.
        totalDistance += length(hitPoint - rayOrigin);
        samples[i] = CalculateSample(hitPoint, vertex, entityID); // TODO add distance to camera?

        // Bounce ray.
        rayDirection = reflect(rayDirection, vertex.norm);
        rayOrigin = hitPoint + rayDirection * 0.01f;
    }

    // Calculate final color.
    float3 finalColor = float3(0.f, 0.f, 0.f);s
    for (int i = numBounces - 1; i >= 0; --i)
    {
        finalColor += (1 - samples[i].diffuseFactor) * samples[i].diffuse + samples[i].diffuseFactor * finalColor;
        //finalColor += float3(samples[i].diffuseFactor, samples[i].diffuseFactor, samples[i].diffuseFactor);
        //finalColor += samples[i].diffuse;
    }

    // Set pixel color.
    g_Target[threadID.xy] = float4(finalColor, 1.f);
}

HitData RayVsScene(float3 rayOrigin, float3 rayDirection) 
{
    HitData hitData;
    hitData.t = FLT_MAX;
    const int numVertices = g_MetaBuffer[0].numOfVertices;
    for (int v = 0; v < numVertices; v += 3) 
    {
        float t = RayVsTriangle(rayOrigin, rayDirection, g_VertexBuffer[v].pos, g_VertexBuffer[v + 1].pos, g_VertexBuffer[v + 2].pos);
        if (t > 0.f && t < hitData.t) 
        {
            hitData.t = t;
            hitData.vertexOffset = v;
        }
    }
    return hitData;
}

float RayVsTriangle(float3 rayOrigin, float3 rayDirection, float3 v0, float3 v1, float3 v2) 
{
    float3 e1 = v1 - v0;
    float3 e2 = v2 - v0;
    float3 s = rayOrigin - v0;
    float3 d = rayDirection * -1.f;
    float det = determinant(float3x3(d, e1, e2));
    if (abs(det) > 0.001f)
    {
        det = 1 / det;
        float u = det * determinant(float3x3(d, s, e2));
        float v = det * determinant(float3x3(d, e1, s));

        if ((u >= -0.001f && u <= 1.001f) && (v >= -0.001f && v <= (1.001f - u)))
        {
            return det * determinant(float3x3(s, e1, e2));
        }
    }
    return -1.f;
}

Vertex Interpolate(float3 hitPoint, Vertex v0, Vertex v1, Vertex v2)
{
    float3 f = hitPoint;
    float3 p0 = v0.pos;
    float3 p1 = v1.pos;
    float3 p2 = v2.pos;

    // Calculate vectors from point f to vertices p0, p1 and p2.
    float3 f0 = p0 - f;
    float3 f1 = p1 - f;
    float3 f2 = p2 - f;
    // Calculate the areas and factors (order of parameters doesn't matter).
    float a = length(cross(p0 - p1, p0 - p2)); // main triangle area a.
    float a0 = length(cross(f1, f2)) / a; // p0's triangle area / a.
    float a1 = length(cross(f2, f0)) / a; // p1's triangle area / a.
    float a2 = length(cross(f0, f1)) / a; // p2's triangle area / a.

    Vertex vertex;
    vertex.pos = v0.pos * a0 + v1.pos * a1 + v2.pos * a2;
    vertex.norm = v0.norm * a0 + v1.norm * a1 + v2.norm * a2;
    vertex.uv = v0.uv * a0 + v1.uv * a1 + v2.uv * a2;

    return vertex;
}

Sample CalculateSample(float3 hitPoint, Vertex vertex, int entityID)
{
    Sample sample;
    sample.diffuse = float3(0.f, 0.f, 0.f);
    sample.diffuseFactor = 1.f;
    int numOfPointLights = g_MetaBuffer[0].numOfPointLights;
    for (int i = 0; i < numOfPointLights; ++i)
    {
        // Point light data.
        PointLight pointLight = g_PointLightBuffer[i];
        float3 lightVec = pointLight.pos - vertex.pos;
        float distance = length(lightVec);
        lightVec = normalize(pointLight.pos - vertex.pos);

        // Check if pixel in shadow.
        HitData hitData = RayVsScene(hitPoint + lightVec * 0.01f, lightVec);

        if (hitData.t >= distance)
        {
            // Calculate color.
            //float3 color = g_DiffuseTexture.SampleLevel(g_SampAni, vertex.uv, 0).xyz;
            float3 diffuse = g_DiffuseTextureArray.SampleLevel(g_SampAni, float3(vertex.uv, entityID), 0).xyz;
            float3 normal = vertex.norm;
            float distanceFactor = 1.f / (distance * distance);
            float lightFactor = dot(lightVec, normal);
            const float reflectionCoefficient = 0.32;
            // Fresnel effect. (Schlick's approximation)
            sample.diffuseFactor = reflectionCoefficient + (1.f - reflectionCoefficient) * pow((1.f - lightFactor), 5.f);
            sample.diffuse += diffuse * pointLight.col * lightFactor * clamp(distanceFactor, 0.f, 1.f);
        }
    }

    return sample;
}

int GetEntityID(int vertexIndex)
{
    int numEntities = g_MetaBuffer[0].numEntities;
    for (int i = 0; i < numEntities; ++i)
    {
        int offset = g_EntityEntries[i].offset;
        int numOfVertices = g_EntityEntries[i].numOfVertices;
        if (vertexIndex >= offset && vertexIndex < offset + numOfVertices)
            return i;
    }
    return -1;
}
