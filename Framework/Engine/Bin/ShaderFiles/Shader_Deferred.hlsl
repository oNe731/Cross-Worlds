#include "Engine_Shader_Defines.hpp"


Texture2D g_Texture; // ����׿� �ؽ���

Texture3D g_PerlinNoiseTextures;

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv;

matrix g_CamProjMatrix;
matrix g_LightViewMatrix;

Texture2D g_DiffuseTarget;
Texture2D g_NormalTarget;
Texture2D g_DepthTarget;
		  
Texture2D g_ShadeTarget;
Texture2D g_ShadowTarget;
Texture2D g_SpecularTarget;

Texture2D g_SSAOTarget;
Texture2D g_BloomTarget;
Texture2D g_OutlineTarget;

Texture2D g_BlendMixTarget;

Texture2D g_BlendTarget;
Texture2D g_DistortionTarget;


// ����
vector g_vCamPosition;


// �Ȱ�
float4 g_vFogColor    = { 0.f, 0.635f, 1.f, 1.f };
float2 g_fFogStartEnd = { 300.f, 600.f };
float g_fFogStartDepth      = 0.f;
float g_fFogStartDistance   = 0.f;
float g_fFogDistanceValue   = 0.f;
float g_fFogHeightValue     = 0.f;
float g_fFogLimit = -1.f;
float2 g_vFogUVAcc = { 0.f, 0.f };
float g_fDistanceDensity = 0.f;
float g_fHeightDensity = 0.f;

// �ɼ�
bool   g_bShadowDraw;
bool   g_bSsaoDraw;
bool   g_bOutLineDraw;

float  g_fBias = 0.2f;

// ���� ��
float g_fQuality = 16.f;
float g_fRadialPower = 0.1f;

// JunYeop
Texture2DArray CascadeShadowMapTexture;
SamplerComparisonState PCFSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
    MaxAnisotropy = 1;
    ComparisonFunc = LESS_EQUAL;
    MaxLOD = 3.402823466e+38f;
};

cbuffer cbDirLightPS : register(b1)
{
    float3 vLightAmbientDown : packoffset(c0);
    float3 vLightAmbientUp : packoffset(c1);
    float3 vDirToLight : packoffset(c2);
    float3 vDirLightColor : packoffset(c3);
    float4x4 ToShadowSpace : packoffset(c4);
    float4 ToCascadeOffsetX : packoffset(c8);
    float4 ToCascadeOffsetY : packoffset(c9);
    float4 ToCascadeScale : packoffset(c10);
}

cbuffer cbPointLightPixel
{
    float3 vPointLightPos;
    float3 vPointColor;
    float fPointLightRangeRcp;
};

cbuffer cbSpotLightConstants
{
    float3 vSpotLightPos;
    float fSpotLightRangeRcp;
    float3 vSpotDirToLight;
    float fSpotCosOuterCone;
    float3 vSpotColor;
    float fSpotCosInnerConeRcp;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float		fFogFactor : FOG;
};



VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

// DEBUG
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

// LIGHT
struct PS_OUT_LIGHT
{
	float4	vShade : SV_TARGET0;	
	float4	vSpecular : SV_TARGET1;
    float4  vShadow : SV_TARGET2;
};

float CascadeShadow(float3 position)
{
    float4 posShadowSpace = mul(float4(position, 1.0f), ToShadowSpace);
    
    float4 posCascadeSpaceX = (ToCascadeOffsetX + posShadowSpace.xxxx) * ToCascadeScale;
    float4 posCascadeSpaceY = (ToCascadeOffsetY + posShadowSpace.yyyy) * ToCascadeScale;
    
    float4 inCascadeX = abs(posCascadeSpaceX) <= 1.0f;
    float4 inCascadeY = abs(posCascadeSpaceY) <= 1.0f;
    float4 inCascade = inCascadeX * inCascadeY;
    
    float4 bestCascadeMask = inCascade;
    bestCascadeMask.yzw = (1.0f - bestCascadeMask.x) * bestCascadeMask.yzw;
    bestCascadeMask.zw = (1.0f - bestCascadeMask.y) * bestCascadeMask.zw;
    bestCascadeMask.w = (1.0f - bestCascadeMask.z) * bestCascadeMask.w;
    float bestCascade = dot(bestCascadeMask, float4(0.0f, 1.0f, 2.0f, 3.0f));

    float3 UVD;
    UVD.x = dot(posCascadeSpaceX, bestCascadeMask);
    UVD.y = dot(posCascadeSpaceY, bestCascadeMask);
    UVD.z = posShadowSpace.z;
    
    UVD.xy = 0.5f * UVD.xy + 0.5f;
    UVD.y = 1.0f - UVD.y;
    
    float shadow = CascadeShadowMapTexture.SampleCmpLevelZero(PCFSampler, float3(UVD.xy, bestCascade), UVD.z);
    
    shadow = saturate(shadow + 1.0f - any(bestCascadeMask));
    
    return shadow;
}

void CaclDirectional(float3 position, float3 normal, float3 ambientColor, inout PS_OUT_LIGHT output)
{
	// Phong Diffuse
    float NDotL = dot(vDirToLight, normal);
    output.vShade = float4(vDirLightColor.rgb * saturate(NDotL) + ambientColor, 1.0f);
    output.vShade = (ceil(output.vShade * 2.f) / 2.f);
	// Blinn Specular
    float3 ToEye = g_vCamPosition.xyz - position;
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + vDirToLight);
    float NDotH = saturate(dot(HalfWay, normal));
	// Pow -> (NDotH, specPow) * SpecIntensity(SpecularMap Sampling)
    output.vSpecular = float4(vDirLightColor.rgb * pow(NDotH, 10.0f) * 1.0f, 1.0f);
    
    float shadowAtt;
    shadowAtt = CascadeShadow(position);
    output.vShadow = float4(shadowAtt, shadowAtt, shadowAtt, 1.0f);
}

float3 CaclAmbient(float3 normal)
{
    float up = normal.y * 0.5f + 0.5f;
	
    float3 ambient = vLightAmbientDown + up * vLightAmbientUp;

    return ambient;
}

void CaclPoint(float3 position, float3 normal, inout PS_OUT_LIGHT output)
{
    float3 ToLight = vPointLightPos - position;
    float3 ToEye = g_vCamPosition.xyz - position;
    float DistToLight = length(ToLight);

	// Phong Diffuse
    ToLight /= DistToLight; // normalize
    float NDotL = saturate(dot(ToLight, normal));
    output.vShade = float4(vPointColor.rgb * saturate(NDotL), 1.0f);
    
	// Blinn Specular
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + ToLight);
    float NDotH = saturate(dot(HalfWay, normal));
    output.vSpecular = float4(vPointColor * pow(NDotH, 10.0f) * 1.0f, 1.0f);
	
	// Attenuation
    float DistToLightNorm = 1.0f - saturate(DistToLight * fPointLightRangeRcp);
    float Attn = DistToLightNorm * DistToLightNorm;
    
    output.vShade *= Attn;
    output.vSpecular *= Attn;
    
    output.vShadow = float4(1.0f, 1.0f, 1.0f, 1.0f);

}

void CaclSpot(float3 position, float3 normal, inout PS_OUT_LIGHT output)
{
    float3 ToLight = vSpotLightPos - position;
    float3 ToEye = g_vCamPosition.xyz - position;
    float DistToLight = length(ToLight);

	// Phong Diffuse
    ToLight /= DistToLight;
    float NDotL = saturate(dot(ToLight, normal));
    output.vShade = float4(vSpotColor.rgb * saturate(NDotL), 1.0f);
	
	// Blinn Specular
    ToEye = normalize(ToEye);
    float3 HalfWay = normalize(ToEye + ToLight);
    float NDotH = saturate(dot(HalfWay, normal));
    output.vSpecular = float4(vSpotColor.rgb * pow(NDotH, 10.0f) * 1.0f, 1.0f);
	
	// Cone Attenuation
    float cosAng = dot(vSpotDirToLight, ToLight);
    float conAtt = saturate((cosAng - fSpotCosOuterCone) * fSpotCosInnerConeRcp);
    conAtt *= conAtt;
	
	// Attenuation
    float DistToLightNorm = 1.0f - saturate(DistToLight * fSpotLightRangeRcp);
    float Attn = DistToLightNorm * DistToLightNorm;
	
    output.vShade *= Attn * conAtt;
    output.vSpecular *= Attn * conAtt;
    
    output.vShadow = float4(1.0f, 1.0f, 1.0f, 1.0f);
}

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN input)
{
    PS_OUT_LIGHT output = (PS_OUT_LIGHT) 0;

    float4 vNormalDesc = g_NormalTarget.Sample(PointSampler, input.vTexcoord);
    float4 vDepthDesc = g_DepthTarget.Sample(PointSampler, input.vTexcoord);
    float fViewZ = vDepthDesc.y * 1000.0f;
	
    float3 vNormal = float3(vNormalDesc.xyz * 2.0f - 1.0f);
	
    float4 vWorldPos;
    vWorldPos.x = input.vTexcoord.x * 2.0f - 1.0f;
    vWorldPos.y = input.vTexcoord.y * -2.0f + 1.0f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;
	
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    float3 ambientColor = CaclAmbient(vNormal);
    CaclDirectional(vWorldPos.xyz, vNormal, ambientColor, output);
	
    return output;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN input)
{
    PS_OUT_LIGHT output = (PS_OUT_LIGHT) 0;

    float4 vNormalDesc = g_NormalTarget.Sample(PointSampler, input.vTexcoord);
    float4 vDepthDesc = g_DepthTarget.Sample(PointSampler, input.vTexcoord);
    float fViewZ = vDepthDesc.y * 1000.0f;
	
    float3 vNormal = float3(vNormalDesc.xyz * 2.0f - 1.0f);
	
    float4 vWorldPos;
    vWorldPos.x = input.vTexcoord.x * 2.0f - 1.0f;
    vWorldPos.y = input.vTexcoord.y * -2.0f + 1.0f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;
	
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    CaclPoint(vWorldPos.xyz, vNormal, output);
	
    return output;
}

PS_OUT_LIGHT PS_MAIN_SPOT(PS_IN input)
{
    PS_OUT_LIGHT output = (PS_OUT_LIGHT) 0;

    float4 vNormalDesc = g_NormalTarget.Sample(PointSampler, input.vTexcoord);
    float4 vDepthDesc = g_DepthTarget.Sample(PointSampler, input.vTexcoord);
    float fViewZ = vDepthDesc.y * 1000.0f;
	
    float3 vNormal = float3(vNormalDesc.xyz * 2.0f - 1.0f);
	
    float4 vWorldPos;
    vWorldPos.x = input.vTexcoord.x * 2.0f - 1.0f;
    vWorldPos.y = input.vTexcoord.y * -2.0f + 1.0f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.0f;
	
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	
    CaclSpot(vWorldPos.xyz, vNormal, output);
	
    return output;
}

PS_OUT PS_RADIAL_BLUR(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

    float4 colour = { 0.f, 0.f, 0.f, 0.f };
    float v = 0.f;

    for (float i = 0.0f; i < 1.0f; i += (1 / g_fQuality))
    {
        v = 0.9 + i * g_fRadialPower;
        colour += g_BlendTarget.Sample(PointSampler, In.vTexcoord * v + 0.5f - 0.5f * v);
    }

    colour /= g_fQuality;
    colour.a = 1.f;
    
    Out.vColor = colour;
    return Out;
}

// SHADOW
float PCF_ShadowCaculation(float4 vLightPos, float3 vLightDir)
{
	float3 projCoords = vLightPos.xyz / vLightPos.w;

	projCoords.x = projCoords.x * 0.5f + 0.5f;
	projCoords.y = projCoords.y * -0.5f + 0.5f;

	float fCurrentDepth = projCoords.z;
	if (fCurrentDepth >= 1.f)
	{
		fCurrentDepth = 1.f;
		return fCurrentDepth;
	}

	float fShadow = 0.0f;
	float2 texelSize = float2(1.f / 1600.f, 1.f / 900.f);
	texelSize /= 3.f;

	[unroll]
	for (int x = -1; x <= 1; ++x)
	{
		[unroll]
		for (int y = -1; y <= 1; ++y)
		{
			float fPCFDepth = g_ShadowTarget.Sample(PointSampler, projCoords.xy + (float2(x, y) * texelSize)).r;
			fShadow += vLightPos.w - g_fBias > fPCFDepth * 1000.f ? 0.5f : 1.0f;
		}
	}
	fShadow /= 9.0f;

	return fShadow;
}

float4 Shadow_Caculation(PS_IN In, float vDepthX, float vDepthY)
{
	vector vWorldPos;
    float fViewZ = vDepthY * 1000.f;

	/* ���������̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthX;
	vWorldPos.w = 1.f;

	/* �佺���̽� ���� ��ġ�� ���Ѵ�. */
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ������� ����. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLightPos = mul(vWorldPos, g_LightViewMatrix);
    
	float3 vLightDir = normalize(float3(0.f, 0.f, 0.f) - vLightPos.xyz);

	vLightPos = mul(vLightPos, g_CamProjMatrix);

	float fShadowColor = PCF_ShadowCaculation(vLightPos, vLightDir);

	return vector(fShadowColor, fShadowColor, fShadowColor, 1.f);
}

// FOG
float DistanceFogFactor_Caculation(float fViewZ)
{
    return saturate((g_fFogStartEnd.y - fViewZ) / (g_fFogStartEnd.y - g_fFogStartEnd.x));
}

float3 Compute_HeightFogColor(float3 vOriginColor, float3 toEye, float fNoise)
{
    // ���� ������ ��ȯ�� Distance..
    float pixelDistance =  g_fDistanceDensity * (length(g_vCamPosition.w - toEye) - g_fFogStartDepth);
    
	// ���� ������ ��ȯ�� Height..
    float pixelHeight =  g_fHeightDensity * toEye.y;
    
    float distanceOffset = min(pow(2.0f, pixelDistance - g_fFogStartDistance), 1.0f);
    float heightOffset = min(pow(1.2f, -(pixelHeight + 3.0f)), 1.0f);
    
	// �Ÿ� ��� �Ȱ� ���� ����..
    float distanceValue = exp(0.01f * pow(pixelDistance - g_fFogDistanceValue, 3.0f));
    float fogDistanceFactor = min(distanceValue, 1.0f);

	// ���� ��� �Ȱ� ���� ����..
    float heightValue = (pixelHeight * g_fFogHeightValue) - 0.1f;
    float fogHeightFactor = pow(pow(2.0f, -heightValue), heightValue) * (1.0f - distanceOffset);

	// �� ��Ҹ� ������ ���� ���..
    float fogFinalFactor = min(fogDistanceFactor * fogHeightFactor * fNoise, 1.0f) + min(distanceOffset * heightOffset, 1.0f) + 0.01f;

	// ���� ȥ�� ����..
    return lerp(vOriginColor.rgb, g_vFogColor.xyz, fogFinalFactor);
}

//float3 ApplyFog(float3 rgb, float distance, float3 vRayOrigin, float3 vRayDir)
//{
//    vRayDir = normalize(vRayDir);
//    float fogAmount = g_fConvertPercent * exp(-vRayOrigin.y * g_fConvertPercent) * (1.f - exp(-distance * vRayDir.y * g_fConvertPercent)) / vRayDir.y;
    
//    return lerp(rgb, g_vFogColor.rgb, fogAmount);
//}

//float3 Gamma_LinearSpace(float3 _rgb)
//{
//    return pow(_rgb, 2.2);
//}


// DEFERRED
PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	// vDiffuse
	vector vDiffuse = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);
    // vDiffuse.rgb = Gamma_LinearSpace(vDiffuse.rgb);
    
	if (vDiffuse.a == 0.f)
		discard;

	// vShade
	vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
	vShade = saturate(vShade);
	
	//vSpecular
    vector vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
    vSpecular = saturate(vSpecular);
	
	// �� �ȼ� ���� �� ��Ÿ ó��
    vector vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
    if (vDepthDesc.w != 1.f) 
        vSpecular = float4(0.f, 0.f, 0.f, 0.f);

	// Shadow
	vector vShadow = float4(1.f, 1.f, 1.f, 1.f);
	if(g_bShadowDraw)
        vShadow = g_ShadowTarget.Sample(PointSampler, In.vTexcoord);

    // SSAO
	vector vSSAO = float4(1.f, 1.f, 1.f, 1.f);
	if(g_bSsaoDraw) 
		vSSAO = g_SSAOTarget.Sample(LinearSampler, In.vTexcoord);

	// Outline
	vector vOutline = float4(1.f, 1.f, 1.f, 1.f);
	if(g_bOutLineDraw)
		vOutline = g_OutlineTarget.Sample(LinearSampler, In.vTexcoord);
	
	// Bloom
    vector vBloom = g_BloomTarget.Sample(LinearSampler, In.vTexcoord);

	// Output
    Out.vColor = ((vDiffuse * vShade * vShadow * vSSAO) + vSpecular + vBloom) * vOutline;
	
	// Fog
    // float fDistanceFogFactor = DistanceFogFactor_Caculation(vDepthDesc.y * 1000.f);
    
    vector vWorldPos;
    float fViewZ = vDepthDesc.y * 1000.f;

	/* ���������̽� ���� ��ġ�� ���Ѵ�. */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;

	/* �佺���̽� ���� ��ġ�� ���Ѵ�. */
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vector vViewPos = vWorldPos;

	/* ������� ����. */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    
    
    //// 0~1000
    //int iIndex = min(vDepthDesc.y * 10.f - 1.f, 9.f);
    
    float3 vTexCoord = float3((vWorldPos.xyz * 100.f) % 12800.f) / 12800.f;
    vTexCoord.x += g_vFogUVAcc.x;
    vTexCoord.y += g_vFogUVAcc.y;
    
    float fNoise = g_PerlinNoiseTextures.Sample(LinearSampler, vTexCoord).r;
    
    float3 vFinalColor = Compute_HeightFogColor(Out.vColor.xyz, (vWorldPos - g_vCamPosition).xyz, fNoise);
    
    Out.vColor = vector(vFinalColor.rgb, 1.f);
	
	return Out;
}

// DEFERRED_UI
PS_OUT PS_MAIN_DEFERRED_UI(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

	// vDiffuse
    vector vDiffuse = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);
    // vDiffuse.rgb = Gamma_LinearSpace(vDiffuse.rgb);
    
    if (vDiffuse.a == 0.f)
        discard;

	// vShade
    vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
    vShade = saturate(vShade);
	
	//vSpecular
    vector vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
    vSpecular = saturate(vSpecular);
	
	// �� �ȼ� ���� �� ��Ÿ ó��
    vector vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
    if (vDepthDesc.w != 1.f) 
        vSpecular = float4(0.f, 0.f, 0.f, 0.f);

	// Outline
    vector vOutline = float4(1.f, 1.f, 1.f, 1.f);
    if (g_bOutLineDraw)
        vOutline = g_OutlineTarget.Sample(LinearSampler, In.vTexcoord);
	
	// Bloom
    vector vBloom = g_BloomTarget.Sample(LinearSampler, In.vTexcoord);

	// Output
    Out.vColor = (vDiffuse * vShade * vOutline) + vSpecular + vBloom;

    return Out;
}

// ALPHABLENDMIX
PS_OUT PS_MAIN_ALPHABLENDMIX(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_BlendMixTarget.Sample(PointSampler, In.vTexcoord);

	return Out;
}

PS_OUT PS_MAIN_SHADOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    vector vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
    if (vDepthDesc.z == 1.f)
        Out.vColor = float4(1.f, 1.f, 1.f, 1.f);
	else 
        Out.vColor = Shadow_Caculation(In, vDepthDesc.x, vDepthDesc.y);

    return Out;
}

PS_OUT PS_DISTORTION(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float2 vTexelSize = float2(1.f / 1600.f, 1.f / 900.f);
	
    float4 vDistortionWeight = g_DistortionTarget.Sample(PointSampler, In.vTexcoord);
	
    float2 vSampleTexCoord = In.vTexcoord + (vDistortionWeight.rg);
	
    Out.vColor = g_BlendTarget.Sample(PointSampler, vSampleTexCoord);
    Out.vColor.a = 1.f;
	
    return Out;
}


technique11 DefaultTechnique
{
	// 0
	pass Target_Debug
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	// 1
	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

	// 2
	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }

	// 3
	pass Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL; 
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
	}

	// 4
	pass AlphaBlendMix
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHABLENDMIX();
	}

    // 5
    pass OneBlendMix
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHABLENDMIX();
    }

    // 6
    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

	
	// 7
    pass Distortion
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_DISTORTION();
    }
		
	// 8
    pass Light_Spot
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPOT();
    }

    // 9
    pass RadialBlur
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_RADIAL_BLUR();
    }

	// 10
    pass Deferred_UI
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_UI();
    }
}