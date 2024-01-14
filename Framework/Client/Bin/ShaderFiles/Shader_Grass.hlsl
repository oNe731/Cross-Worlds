#include "Engine_Shader_Defines.hpp"

struct VertexInput
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};

struct HullInput
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};

// Vertex
HullInput ColorVertexShader(VertexInput input)
{
    HullInput output;
    
    // ���� ��ġ�� ��ü ���̴��� ����.
    output.vPosition = input.vPosition;
    
    // �ȼ� ���̴��� ����� �Է� ������ �����Ѵ�.
    output.vColor = input.vColor;
    
    return output;
}

cbuffer TessellationBuffer
{
    float fTessellationAmount;
};

struct ConstantOutput
{
    float fEdges[3] : SV_TessFactor;
    float fInside : SV_InsideTessFactor;
};

struct HullOutput
{
    float3 vPosition : POSITION;
    float4 vColor : COLOR;
};

// �� ���̴����� ����� �Լ�
ConstantOutput ConstantHullShader(InputPatch<HullInput, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutput output = (ConstantOutput) 0;
    
    // SV_TessFactor�� �ﰢ���� �� ���� �� ���� ��������. -> fEdges
    output.fEdges[0] = fTessellationAmount;
    output.fEdges[1] = fTessellationAmount;
    output.fEdges[2] = fTessellationAmount;
    
    // Ʈ���̾ޱ� ���θ� �ͼ����̼� �ϱ����� �ͼ����̼� ��� ����
    // ���� ��Ʈ�� ����Ʈ�� �����Ѵ�.
    output.fInside = fTessellationAmount;

    
    return output;
}

////
//HullShader
////

[domain("tri")] // ��ġ�� ����, �ﰢ���� tri( tri, quad , isoline )
[partitioning("integer")] // �׼����̼� ����ȭ ��� (integer : ����, fractional_even, fractional_odd)
[outputtopology("triangle_cw")] // ���� ������� �ﰢ������ ����� ���� ( cw : �ð���� , ccw : �ݽð� line : �� �׼����̼�  )
[outputcontrolpoints(3)] // �Էµ� ��Ʈ�� ����Ʈ ����
[patchconstantfunc("ConstantHullShader")] // constant Hull Shader �Լ� �̸�
HullOutput ColorHullShader(InputPatch<HullInput, 3> patch, uint pointId : SV_OutputControlPointID,
uint patchId : SV_PrimitiveID)
{
    HullOutput output = (HullOutput) 0;

    // �������� ��ġ�� ��� ��ġ�� ����
    output.vPosition = patch[pointId].vPosition;
    
    // �Է� ������ ��� �������� ����
    output.vColor = patch[pointId].vColor;
    
    return output;
}

cbuffer MatrixBuffer
{
    matrix worldMatrix, viewMatrix, projectionMatrix;
};

struct PixelInput
{
    float4 vPosition : SV_POSITION;
    float4 vColor : COLOR;
};

// Domain Shader

// ������ũ ó���� �����͸� �޾Ƽ� ���ؽ� ���̴��� ������ ����� �Ͱ� ���� 
// ���� �������� �����ϰ� ��ȯ�ϴµ� ���

[domain("tri")]
PixelInput ColorDomainShader(ConstantOutput input, float3 uvwCoord : SV_DomainLocation,
const OutputPatch<HullOutput, 3> patch)
{
    float3 vertexPosition;
    PixelInput output = (PixelInput) 0;

    vertexPosition = uvwCoord.x * patch[0].vPosition + uvwCoord.y * patch[1].vPosition +
    uvwCoord.z * patch[2].vPosition;
    
    matrix matWV, matWVP;
    
    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);
    
    output.vPosition = mul(float4(vertexPosition, 1.0f), matWVP);
    
    output.vColor = patch[1].vColor;
    
    return output;
}

// PixelShader
struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vSunMask : SV_TARGET4;
};

PS_OUT ColorPixelShader(PixelInput input)
{
    PS_OUT output = (PS_OUT) 0;
    
    output.vColor = input.vColor;
    output.vSunMask = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return output;
}

RasterizerState RS_WireFrame
{
    FillMode = WireFrame;
    CullMode = Back;
    FrontCounterClockwise = false;
};

technique11 DefaultGrass
{
	// Grass
    pass RealTimeGrass
    {
        SetRasterizerState(RS_WireFrame);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 ColorVertexShader();
        GeometryShader = NULL;
        HullShader = compile hs_5_0 ColorHullShader();
        DomainShader = compile ds_5_0 ColorDomainShader();
        PixelShader = compile ps_5_0 ColorPixelShader();
    }
}