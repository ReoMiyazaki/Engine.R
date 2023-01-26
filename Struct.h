#pragma once
// 定数バッファ用データ構造体(3D変換行列)
struct ConstBufferDataTransform {
	XMMATRIX mat;	// 3D変換行列
};

// 頂点データ構造体
struct Vertex
{
	XMFLOAT3 pos;		// xyz座標
	XMFLOAT3 normal;	// 法線ベクトル
	XMFLOAT2 uv;		// cv座標
};

// 定数バッファ用データ構造体(マテリアル)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // 色(RGBA)
};


