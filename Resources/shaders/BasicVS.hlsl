#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;	// ピクセルシェーダーに渡す値
//	output.svpos = pos;
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
//	return pos;	// + float4( Xオフセット, Yオフセット, 0, 0 )指定しただけずれる / * float4( X倍率, Y倍率, 1, 1 )指定した倍率で拡縮する
//}