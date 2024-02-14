#include "Engine_Shader_Defines.hpp"

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer SkyBuffer
{
    float fTranslation;
    float fScale;
    float fbrightness;
    float padding;
};

//Texture2D cloudTexture1 : register(t0);
//Texture2D cloudTexture2 : register(t1);

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
    
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

VS_OUT SkyPlaneVertexShader(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);
    
    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vTexcoord = input.vTexcoord;

    return output;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
    
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET;
};

Texture2D cloudTexture : register(t0);
Texture2D perturbTexture : register(t1);


SamplerState ClampState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
};

PS_OUT SkyPlanePixelShader(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 perturbValue;
    float4 cloudColor;
    
    input.vTexcoord.x = input.vTexcoord.x + fTranslation;
    
    perturbValue = perturbTexture.Sample(LinearSampler, input.vTexcoord * 2);
    
    perturbValue = perturbValue * fScale;
    
    perturbValue.xy = perturbValue.xy + input.vTexcoord.xy;
    
    cloudColor = cloudTexture.Sample(LinearSampler, input.vTexcoord * 2);
    
    cloudColor = cloudColor * fbrightness;
    
    output.vColor = cloudColor;
    //output.vColor = perturbValue;
    return output;
}

Texture2D WinterNoiseTexture : register(t2);
Texture2D WinterNoiseTexture2 : register(t3);
Texture2D WinterAuraTexture : register(t4);

PS_OUT WinterSkyPlanePixelShader(PS_IN input)
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 perturbValue;
    float4 cloudColor;
    float4 cloudColor2;
    

    float noiseValue = WinterNoiseTexture2.Sample(LinearSampler, input.vTexcoord * 12.0).r;
    
   if(noiseValue > 0.9)
       output.vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
   else
       output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return output;
}

RasterizerState RS_SkyPlane
{
    FillMode = SOLID;

	/* �ո��� �ø��ϰڴ�. == �ĸ��� �����ְڴ�. */
    CullMode = Back;

	/* �ո��� �ð�������� ���ڴ�. */
    FrontCounterClockwise = false;
};

BlendState BS_Blend
{
    BlendEnable[0] = true;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;
    BlendOp[0] = ADD;
    SrcBlendAlpha[0] = ONE;
    DestBlendAlpha[0] = Zero;
    BlendOpAlpha[0] = ADD;
    RenderTargetWriteMask[0] = 0x0f;
};

technique11 CloudDefault
{
    pass SkyPlane
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 SkyPlaneVertexShader();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 SkyPlanePixelShader();
    }

    pass WinterSkyPlane
    {
        SetRasterizerState(RS_SkyPlane);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 SkyPlaneVertexShader();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 WinterSkyPlanePixelShader();
    }
}