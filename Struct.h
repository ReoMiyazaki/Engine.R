#pragma once
// �萔�o�b�t�@�p�f�[�^�\����(3D�ϊ��s��)
struct ConstBufferDataTransform {
	XMMATRIX mat;	// 3D�ϊ��s��
};

// ���_�f�[�^�\����
struct Vertex
{
	XMFLOAT3 pos;		// xyz���W
	XMFLOAT3 normal;	// �@���x�N�g��
	XMFLOAT2 uv;		// cv���W
};

// �萔�o�b�t�@�p�f�[�^�\����(�}�e���A��)
struct ConstBufferDataMaterial {
	XMFLOAT4 color; // �F(RGBA)
};


