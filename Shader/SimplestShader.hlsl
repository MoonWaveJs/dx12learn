
cbuffer cGlogalObjectBuffer : register(b0)
{
    float4x4 gViewProj;
}

cbuffer cbPerObjectBuffer : register(b1)
{
    float4x4 WorldViewProj;
}

struct VsIn
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float4 Color    : COLOR;
    float4 Uv       : TEX0;
};

struct VsOut
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
};

VsOut VS(VsIn In)
{
    VsOut Out;
    Out.Position = mul(WorldViewProj,float4(In.Position,1));
    Out.Color    = In.Color;
    return Out;
}

float4 PS(VsOut Out) : SV_TARGET
{
    return Out.Color;
}