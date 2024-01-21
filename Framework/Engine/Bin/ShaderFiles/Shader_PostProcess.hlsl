#include "Engine_Shader_Defines.hpp"
#define FLT_MAX 3.402823466e+38F

Texture2D SunOccluderTexture : register(t0);


cbuffer MatrixBuffer : register(b0)
{
    matrix world, view, projection, worldInvTranspose;
}

cbuffer GodRayBuffer : register(b0)
{
    float4 vLightShaftValue = float4(0.975, 0.25f, 0.825f, 2.0f);
    float2 vScreenSunPosition;
}


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);

    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vTexcoord = input.vTexcoord;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

sampler LinearSamplerClamp = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
    MaxAnisotropy = 16;
    MaxLOD = FLT_MAX;
    ComparisonFunc = NEVER;

};

PS_OUT PS_LIGHTSHAFT(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
    float2 vTexcoord = input.vTexcoord;
    
    float4 lightData = vLightShaftValue;
    // x = Density, y = Weight, z = Decay , w = Exposure
    
    // MaskColor
    float3 vMaskColor = SunOccluderTexture.SampleLevel(LinearSamplerClamp, vTexcoord, 0).rgb;
    float2 lightPosition = vScreenSunPosition;
    float2 deltaTexcoord = (vTexcoord - lightPosition);
    #define NUM_SAMPLERS 64
    
    deltaTexcoord *= lightData.x / NUM_SAMPLERS; // Density / NUM_SAMPLERS
    
    float illuminationDecay = 1.0f;
    float3 accumulatedGodRays = float3(0.0f, 0.0f, 0.0f);
    float3 accumulated = 0.0f;
    
    for (int i = 0; i < NUM_SAMPLERS; ++i)
    {
        vTexcoord.xy -= deltaTexcoord;
        float3 sample = SunOccluderTexture.SampleLevel(LinearSamplerClamp, vTexcoord.xy, 0).rgb;
        sample *= illuminationDecay * lightData.y; // illuminationDecay * Weight
        accumulated += sample;
        illuminationDecay *= lightData.z; // illuminationDecay *= lightData.z (Decay)
    }
    
    accumulated *= lightData.w;
    output.vColor = saturate(float4(vMaskColor + accumulated, 1.0f));
    
   return output;
}

BlendState AdditiveBlendState
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = true;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;
    BlendOp[0] = ADD;
    RenderTargetWriteMask[0] = 15;
};

//SSAO
Texture2D NormalDepthMap : register(t1);
Texture2D RandomVecMap : register(t2);
SamplerState SampRandVec : register(s0)
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
    ComparisonFunc = NEVER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MinLOD = 0.0f;
    MaxLOD = FLT_MAX;
};
SamplerState SampNormalDepth : register(s1)
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = Border;
    AddressV = Border;
    AddressW = Border;
    ComparisonFunc = NEVER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f); // ū ���� ������ ����
    MinLOD = 0.0f;
    MaxLOD = FLT_MAX;
};

cbuffer CBFrustum : register(b1)
{
    float4 FrustumCorner[4];
};

cbuffer CBPerFrame : register(b2)
{
    float4 offsetVectors[14];
    
    // ī�޶� ������ �־��� ��ǥ
    float OcclusionRadius = 0.5f;
    float OcclusionFadeStart = 0.2f;
    float OcclusionFadeEnd = 2.0f;
    float SurfaceEpsilon = 0.05f;
};

struct VertexIn
{
    float3 vPositionLocal : POSITION;
    float3 ToFarPlaneIndex : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VertexOut
{
    float4 vPosition : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

VertexOut VS_SSAO(VertexIn input)
{
    VertexOut output = (VertexOut) 0;

    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);
    // ���� �������� Ư�������� ���� ���� ������ ����Ʈ ��ü�� ���� NDC ��ǥ�� ��ȯ
    output.vPosition = mul(float4(input.vPositionLocal, 1.0f), matWVP);
    
    // �������� �� ���Ÿ� Ŭ���� ����� �� �ȼ������� ���͸� ��� ���� ������ �����ϰ� �ϱ� ����
    // �� ī�޶� �������� �������� ���Ÿ� Ŭ���� ����� �𼭸������� ���͸� ã�´�.
    output.ToFarPlane = FrustumCorner[input.ToFarPlaneIndex.x].xyz;
    
    output.vTexcoord = input.vTexcoord;
    
    return output;
}

// ���հ��� ã�� ���
float OcclusionFunction(float distZ)
{
    //
    //    /|\ Occlusion
    // 1.0 |      ---------------\
    //     |      |             |  \
    //     |                         \
    //     |      |             |      \
    //     |                             \
    //     |      |             |          \
    //     |                                 \
    // ----|------|-------------|-------------|-------> zv
    //     0     Eps          zStart         zEnd

    float occlusion = 0.0f;
    
    // �ʹ� ������ �� ���� ���� ��鿡 ������ ����� ����ų �� ���� ������ �����Ѵ�.
    if (distZ > SurfaceEpsilon)
    {
        float fadeLength = OcclusionFadeEnd - OcclusionFadeStart;
        
        // DistZ�� �����Կ� ���� ����� ���� 1���� 0���� ���������� �����Ѵ�.
        occlusion = saturate((OcclusionFadeEnd - distZ) / fadeLength);
    }
    
    return occlusion;
}

float4 PS_SSAO(VertexOut input) : SV_Target
{
    // p : �� p�� AmbientOcclusion ���� ����Ѵ�.
    // n : �� p���� ���� ����
    // q : �� p�κ����� ������ ����
    // r : p�� ��ȣ�ϰ� �� �� �ִ� �������� ��
    
    // ī�޶� ������ �ȼ��� ���� ���Ϳ� Z ���� ������´�.
    // ScreenQuad�� ��ǥ�� �̹� Texture ��ǥ������ �ִ�.
    float4 normalDepth = NormalDepthMap.SampleLevel(SampNormalDepth, input.vTexcoord, 0.0f);
    
    // ī�޶� ������ �ȼ���ǥ�� Z ��
    float3 n = normalDepth.xyz;
    float pz = normalDepth.w;
    
    // ���� �ﰢ���� ������ ���, ī�޶� �������� �ȼ��� ��ġ�� �籸���Ѵ�.
    // �ش� �������� �� Ŭ���� ������ ������ �� P�� ����Ѵ�.
    // P = t * input.ToFarPlane�� �ȴ�.
    // ���� pz = t * input.ToFarPlane.z�� �ȴ�.
    // pz = t * input.ToFarPlane.z�� �ְ�
    // t = pz / input.ToFarPlane.z�� �ִ�.
    // P = (Pz/input.ToFarPlane), z) * input.ToFarPlane
    
    float3 p = (pz / input.ToFarPlane.z) * input.ToFarPlane;
    
    // ���� ���� ���� ���� ���ϰ� ���� [0, 1]���� [-1, 1] ������ �����Ѵ�.
    // ���� ���� Ÿ�ϸ��� ����.
    float3 randVec = 2.0f * RandomVecMap.SampleLevel(SampRandVec, 4.0f * input.vTexcoord, 0.0f).rgb - 1.0f;
    
    // ��� ���� ��
    float occlusionSum = 0.0f;
    
    // ���� �� ����
    const int sampleCount = 14;
   
    // n ���� �������� �� p�� �ݱ� ������ �ִ� ���� ���� ���ø��ϴ� ���� �Ϲ������� 8/16/32�� ����������.
    // ���⼭�� 14���� ���� ����, �� 14���� ���ø� ���� ���� �ȴ�.
    [unroll]
    for (int i = 0; i < sampleCount; ++i)
    {
        // ���� ���ʹ� �����ǰ� �����ϰ� �л�ȴ�
        // �ֳ��ϸ� ������ ���� ���⿡�� �����ϱ� ������ ���� ���͸� ����Ͽ� �ݿ��ϸ� �������� ������ ���� �л� ���͸� ��´�.
        // ������ ���͸� ����Ͽ� �л��ϰ� �����Ѵ�.
        // ť�� ���ʹ� ���� ���ͷ� ���Ǿ� �ݻ� ���͸� ���� ���� ���ͷ� ã�´�.
        float3 offset = reflect(offsetVectors[i].xyz, randVec);
        
        // ���� ���Ͱ� ǥ�� �ڿ� �ִ� ��� Flip�� �� q�� �׻� ������ ����� p���鿡 �ֵ��� ���� ���͸� �����´�.
        // sign()�� ���� ��ȣ�� ã�´�.
        float flip = sign(dot(offset, n));
        
        // �� p�� ���չݰ濡�� �� q�� ã�´�.
        float3 q = p + flip * OcclusionRadius * offset;
        
        // q�� �����ϰ� ������ �ؽ��� ��ǥ�� ����.
        // q ��ü�� ī�޶� ������ �����Ƿ� ������ �ؽ��� ��ǥ�� ����ȴ�.
        // ���� ������ �������� ����Ǿ�� �Ѵ�.
        float4 ProjQ = mul(float4(q, 1.0f), projection);
        ProjQ /= ProjQ.w;

        // �������� q������ ī�޶� �������� NormalDepthMap�� �ش��ϴ� �ּ� ���� ���� ã�´�.
        // �� �ּ� ���̴� ī�޶� ������ q ���̿� ���� �ʴ�.
        float rz = NormalDepthMap.SampleLevel(SampNormalDepth, ProjQ.xy, 0.0f).w;

        // �ٽ� ���� �ﰢ�� ������ ����Ѵ�.
        float3 r = (rz / q.z) * q;

        // dot(n, normalize(rp))�� �� p ���� �� r�� ��� ���� ����Ѵ�. ������ ���� ���� ��� ���� Ŀ����,
        // r�� p�� Z�Ÿ����� �������� ����� Ŀ����.(�Ӱ谪�� �����)
        // ���� �������� �����ϱ� ���� ��� ���� 0�̴�.
        float distZ = p.z - r.z;
        float dp = max(dot(normalize(n), normalize(r - p)), 0.0f);
        float occlusion = dp * OcclusionFunction(distZ);
        
        occlusionSum += occlusion;
    }
    
    // ���� ����� ��ֶ�����
    occlusionSum /= (float) sampleCount;
    
    // AmbientLight ��� �� ã��
    float access = 1.0f - occlusionSum;
    
    // SSAOMap�� ȿ���� �����ϰ� �ҷ��� pow�� ���
    // SSAOMap�� ȸ�����̴�.
    return saturate(pow(access, 4.0f));
}

technique11 DefaultTechnique
{
    pass GodRay
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(AdditiveBlendState, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LIGHTSHAFT();
        ComputeShader = NULL;
    }

    pass SSAO
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_SSAO();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO();
        ComputeShader = NULL;
    }
}