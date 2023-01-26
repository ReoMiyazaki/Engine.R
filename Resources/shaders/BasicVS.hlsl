#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;	// �s�N�Z���V�F�[�_�[�ɓn���l
//	output.svpos = pos;
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
//	return pos;	// + float4( X�I�t�Z�b�g, Y�I�t�Z�b�g, 0, 0 )�w�肵����������� / * float4( X�{��, Y�{��, 1, 1 )�w�肵���{���Ŋg�k����
//}