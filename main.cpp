#include "Input.h"
#include "WinApp.h"
#include "DirectCommon.h"

#include "Global.h"
#include "Struct.h"
#include <random>
#include <wrl.h>
using namespace Microsoft::WRL;

// 3D�I�u�W�F�N�g�^
struct Object3d
{
	// �萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform = {};
	// �萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform = {};
	// �A�t�B���ϊ����
	XMFLOAT3 scale = { 1, 1, 1 };
	XMFLOAT3 rotation = { 0, 0, 0 };
	XMFLOAT3 position = { 0, 0, 0 };
	// ���[���h�ϊ��s��
	XMMATRIX matWorld = {};
	// �e�I�u�W�F�N�g�ւ̃|�C���^
	Object3d* parent = nullptr;
};

// 3D�I�u�W�F�N�g�̐�
const size_t kObjectount = 50;
// 3D�I�u�W�F�N�g�̔z��
Object3d object3ds[kObjectount];

// 3D�I�u�W�F�N�g������
void InitializeObject3d(Object3d* object, ID3D12Device* device);
// 3D�I�u�W�F�N�g�X�V����
void UpdataObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection);
// 3D�I�u�W�F�N�g�`�揈��
void DrawObject3D(
	Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);

// �E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		//�E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	// �|�C���^
	WinApp* winApp = nullptr;
	// windowAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	// �|�C���^
	DirectCommon* dxCommon = nullptr;
	// DirectX�̏�����
	dxCommon = new DirectCommon();
	dxCommon->Initialize(winApp);

	MSG msg{};	//	���b�Z�[�W

#pragma region	����������
	// �����V�[�h������
	std::random_device seed_gen;
	// �����Z���k�E�c�C�X�^�[
	std::mt19937_64 engine(seed_gen());
	// �����͈�(��]�p�p)
	std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	// �����͈�(���W�p)
	std::uniform_real_distribution<float> posDist(-75.0f, 75.0f);
#pragma endregion

#pragma region DirectX����������

	HRESULT result;
	ComPtr<ID3D12Device> dev;
	
	


#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(dev->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION);
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR);

	}
	D3D12_MESSAGE_ID denyIds[] = {
		/*
		 * Windows11�ł�DXGI�f�o�b�N���C���[��DX12�f�o�b�O���C���[�̑��ݍ�p�o�O�ɂ��G���[���b�Z�[�W
		 *�@https://stackoverflow.com/question/69805245/directx-12-application-is-crashing-in-windows-11
		*/
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};
	// �}������\�����x��
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	// �w�肵���G���[�̕\����}������
	infoQueue->PushStorageFilter(&filter);
#endif


#pragma region 06_01 �[�x�e�X�g

#pragma endregion

	// �|�C���^
	Input* input = nullptr;

	// ���͂̏�����
	input = new Input();
	input->Initialize(winApp);


	// DirectX���������� �����܂�
#pragma endregion

#pragma region �`�揈������������

	Vertex vertices[] =
	{
		//	x		y		z		u		v
		// �O
		{{-5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},	// ����
		{{-5.0f, 5.0f,-5.0f }, {},{0.0f, 0.0f}},	// ����
		{{ 5.0f,-5.0f,-5.0f }, {},{1.0f, 1.0f}},	// �E��
		{{ 5.0f, 5.0f,-5.0f }, {},{1.0f, 0.0f}},	// �E��

		// ��
		{{ 5.0f,-5.0f, 5.0f }, {},{0.0f, 1.0f}},	// ����
		{{ 5.0f, 5.0f, 5.0f }, {},{0.0f, 0.0f}},	// ����
		{{-5.0f,-5.0f, 5.0f }, {},{1.0f, 1.0f}},	// �E��
		{{-5.0f, 5.0f, 5.0f }, {},{1.0f, 0.0f}},	// �E��

		// ��
		{{-5.0f,-5.0f, 5.0f }, {},{0.0f, 1.0f}},	// ����
		{{-5.0f, 5.0f, 5.0f }, {},{0.0f, 0.0f}},	// ����
		{{-5.0f,-5.0f,-5.0f }, {},{1.0f, 1.0f}},	// �E��
		{{-5.0f, 5.0f,-5.0f }, {},{1.0f, 0.0f}},	// �E��

		// �E
		{{ 5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},	// ����
		{{ 5.0f, 5.0f,-5.0f }, {}, {0.0f, 0.0f}},	// ����
		{{ 5.0f,-5.0f, 5.0f }, {}, {1.0f, 1.0f}},	// �E��
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},	// �E��

		// ��
		{{-5.0f, 5.0f,-5.0f }, {}, {0.0f, 1.0f}},	// ����
		{{-5.0f, 5.0f, 5.0f }, {}, {0.0f, 0.0f}},	// ����
		{{ 5.0f, 5.0f,-5.0f }, {}, {1.0f, 1.0f}},	// �E��
		{{ 5.0f, 5.0f, 5.0f }, {}, {1.0f, 0.0f}},	// �E��

		// ��
		{{ 5.0f,-5.0f,-5.0f }, {}, {0.0f, 1.0f}},	// ����
		{{ 5.0f,-5.0f, 5.0f }, {}, {0.0f, 0.0f}},	// ����
		{{-5.0f,-5.0f,-5.0f }, {}, {1.0f, 1.0f}},	// �E��
		{{-5.0f,-5.0f, 5.0f }, {}, {1.0f, 0.0f}},	// �E��
	};

	// �C���f�b�N�X�f�[�^
	unsigned short indicse[] =
	{
		// �O
		0,1,2,	// �O�p�`1��
		2,1,3,	// �O�p�`2��
		// ��
		4,5,6,	// �O�p�`3��
		6,5,7,	// �O�p�`4��
		// ��
		8,9,10,	// �O�p�`5��
		10,9,11,	// �O�p�`6��
		// �E
		12,13,14,	// �O�p�`7��
		14,13,15,	// �O�p�`8��
		// ��
		16,17,18,	// �O�p�`9��
		18,17,19,	// �O�p�`10��
		// ��
		20,21,22,	// �O�p�`11��
		22,21,23,	// �O�p�`12��
	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};		// �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;	// ���_�f�[�^�S�ẴT�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff;
	result = dev->CreateCommittedResource(
		&heapProp,	// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indicse));

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	// �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff;
	result = dev->CreateCommittedResource(
		&heapProp,	// �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	// ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indicse); i++)
	{
		indexMap[i] = indicse[i];	// �C���f�b�N�X���R�s�[
	}
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma region 06_03_�V�F�[�f�B���O

	for (int i = 0; i < _countof(indicse) / 3; i++)
	{	// �O�p�`���ƂɌv�Z���Ă���
		// �O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indicse[i * 3 + 0];
		unsigned short index1 = indicse[i * 3 + 1];
		unsigned short index2 = indicse[i * 3 + 2];

		// �O�p�`���\�����钸�_���W�x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		// p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// �O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// ���K��(������1�ɂ���)
		normal = XMVector3Normalize(normal);

		// ���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

#pragma endregion

	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i];	// ���W���R�s�[
	}

	// �Ȃ��������
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;

	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	ComPtr<ID3DBlob> vsBlob;	// ���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;	// �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�[�I�u�W�F�N�g�̓ǂݍ���
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl",	// �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// �C���N���[�h���\�ɂ���
		"main", "vs_5_0",									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {

		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl",	// �V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					// �C���N���[�h���\�ɂ���
		"main", "ps_5_0",									// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	// �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result)) {

		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	// xyz���W(1�s�ŏ������������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0,
		},
		{	// �@���x�N�g��(1�s�ŏ������������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	// uv���W(1�s�ŏ������������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};


	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	// �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	// �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// MODE_SOLID�h��Ԃ�, MODE_WIREFRAME���ƃ��C���[�t���[��
	pipelineDesc.RasterizerState.DepthClipEnable = true;			// �[�x�N���b�s���O��L����

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RGBA�̑S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;					// �u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// �f�X�g�̒l��  0%�g��


	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// �\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-�\�[�X�̃A���t�@�l

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;	// �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;	// 1�s���Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	// ��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	// �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// �萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[0].Descriptor.ShaderRegister = 0;					// �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_�[���猩����
	
	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			// �f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						// �f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				// �S�ẴV�F�[�_�[���猩����
	
	// �萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
	rootParams[2].Descriptor.ShaderRegister = 1;					// �萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_�[���猩����

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// �c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �|�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�j�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												// �~�j�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;	// �[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		// ��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g

	// �p�C�v���C���X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	result = dev->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));



	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	// 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial;
	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//�}�b�s���O
	assert(SUCCEEDED(result));

	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);

	// �z����̑S�I�u�W�F�N�g�ɑ΂���
	for (int i = 0; i < _countof(object3ds); i++) {

		// ������
		InitializeObject3d(&object3ds[i], dev.Get());

		// �������火�͐e�q�\���̃T���v��
		if (i > 0) { // �擪�ȊO�Ȃ�

			// ��O�̃I�u�W�F�N�g��e�I�u�W�F�N�g�Ƃ���
//			object3ds[i].parent = &object3ds[i - 1];

			// �e�I�u�W�F�N�g��9���̑傫��
			object3ds[i].scale = { 1.0f,1.0f,1.0f };

			// �e�I�u�W�F�N�g�ɑ΂���Z�������30�x��]
			object3ds[i].rotation = { rotDist(engine), rotDist(engine), rotDist(engine) };

			// �e�I�u�W�F�N�g�ɑ΂���Z������-8.0f���炷
			object3ds[i].position = { posDist(engine), posDist(engine), 25.0f };
		}
	}

#pragma region 05_03 �ˉe�ϊ�
	// �������e�ϊ�
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),				// �㉺��p45�x
			(float)WinApp::window_width / WinApp::window_hright,	// �A�X�y�N�g��(��ʉ���/��ʏc��)
			0.1f, 1000.0f							// �O�[�A���[
		);
#pragma endregion

#pragma region 05_04 �r���[�ϊ�
	// �r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);	// ���_���W
	XMFLOAT3 target(0, 0, 0);	// �����_���W
	XMFLOAT3 up(0, 1, 0);		// ������x�N�g��
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	// �J�����̉�]�p
	float angle = 0.0f;
#pragma endregion

	for (size_t i = 0; i < _countof(object3ds); i++)
	{
		UpdataObject3d(&object3ds[i], matView, matProjection);
	}

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/mario.png",	// �uResources�v�t�H���_�́uice.png�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	ScratchImage mipChain{};
	// �~�j�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;	// ��
	textureResourceDesc.Height = (UINT)metadata.height;	// ����
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;
	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));

	// �S�~�j�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�j�}�b�v���x�����w�肵�ăC���[�W�擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^���M
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,				// �S�̈�փR�s�[
			img->pixels,			// ���f�[�^�A�h���X
			(UINT)img->rowPitch,	// 1���C���T�C�Y
			(UINT)img->slicePitch	// 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/reimu.png",	// [Resources]�t�H���_��[texture.png]
		WIC_FLAGS_NONE,
		&metadata2, scratchImg2);

	ScratchImage mipChain2{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg2.GetImages(), scratchImg2.GetImageCount(), scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain2);
	if (SUCCEEDED(result)) {
		scratchImg2 = std::move(mipChain2);
		metadata2 = scratchImg2.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata2.format = MakeSRGB(metadata2.format);

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp2{};
	textureHeapProp2.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp2.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp2.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;
	textureResourceDesc2.Height = (UINT)metadata2.height;
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff2;
	result = dev->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));
	assert(SUCCEEDED(result));

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img2 = scratchImg2.GetImage(i, 0, 0);

		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,		// �S�̈�փR�s�[
			img2->pixels,	// ���f�[�^�A�h���X
			(UINT)img2->rowPitch,	// 1���C���T�C�Y
			(UINT)img2->slicePitch	// �S�T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �f�X�^�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// �V�F�[�_�[���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = dev->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	// SRV�q�[�v�̐擪�o���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// �ݒ�\����
	srvDesc.Format = resDesc.Format;	// RGBA float
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;
	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

	// ��n���h����i�߂�
	UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvHandle.ptr += incrementSize;

	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};		// �ݒ�\����
	srvDesc2.Format = textureResourceDesc2.Format;// RGBA float
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

#pragma endregion

	while (true) {
#pragma region �E�B���h�E���b�Z�[�W����
		// ���b�Z�[�W����
		if (winApp->ProcessMessage())
		{
			// �Q�[�����[�v�𔲂���
			break;
		}
#pragma endregion

#pragma region DirectX���t���[������

		input->Update();

		// ������0�L�[��������Ă�����
		if (input->TriggerKey(DIK_0)) {
			OutputDebugStringA("Hit 0\n");	// �o�̓E�B���h�E�ɁuHIt 0�v�ƕ\��
		}

		// ESC���������Ƃ��Q�[�����[�v�𔲂���
		if (input->PushKey(DIK_ESCAPE)) { break; }

#pragma region �^�[�Q�b�g�̎�������J����

		// �^�[�Q�b�g�̎�������J����
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			if (input->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (input->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			// angle���W�A������Y������ɉ�]�B���a��-100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			// �r���[�ϊ��s�����蒼��
			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

#pragma endregion
#pragma region �ړ�����

		// �����ꂩ�̃L�[�������Ă�����
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			if (input->PushKey(DIK_UP)) { object3ds[0].position.z += 1.0f; }
			else if (input->PushKey(DIK_DOWN)) { object3ds[0].position.z -= 1.0f; }
			if (input->PushKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
			else if (input->PushKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }
		}
		for (size_t i = 0; i < _countof(object3ds); i++)
		{
			UpdataObject3d(&object3ds[i], matView, matProjection);
		}

#pragma endregion

#pragma region 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		// �o�b�N�o�b�t�@�̔ԍ����Ǘ�(2�Ȃ̂�)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();			// �o�b�N�o�b�t�@���w��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// �\����Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �`���Ԃ�
		commandList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 2.�`���̕ύX
		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		// �[�x�X�e���V���r���[�p�f�X�N���v�^�[�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
#pragma endregion

#pragma region 3.��ʃN���A
		FLOAT clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f };	// ���ۂ��F
		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

#pragma region 4.�`��R�}���h

		// �s���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		viewport.Width = WinApp::window_width;		// ����
		viewport.Height = WinApp::window_hright;	// �c��
		viewport.TopLeftX = 0;				// ����X
		viewport.TopLeftY = 0;				// ����Y
		viewport.MinDepth = 0.0f;			// �ŏ��k�x(0�ł悢)
		viewport.MaxDepth = 1.0f;			// �ő�k�x(1�ł悢)

		// �s���[�|�[�g�ݒ�R�}���h���R�}���h���X�g�ɐς�
		commandList->RSSetViewports(1, &viewport);

		// �V�U�[��`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// �؂蔲�����W��
		scissorRect.right = scissorRect.left + WinApp::window_width;	// �؂蔲�����W�E
		scissorRect.top = 0;									// �؂蔲�����W��
		scissorRect.bottom = scissorRect.top + WinApp::window_hright;	// �؂蔲�����W��

		// �V�U�[��`�ݒ���R�}���h���X�g�ɐς�
		commandList->RSSetScissorRects(1, &scissorRect);

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		commandList->SetPipelineState(pipelineState.Get());
		commandList->SetGraphicsRootSignature(rootSignature.Get());

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// �O�p�`���X�g

		// ���_�o�b�t�@�r���[�̐ݒ�R�}���h
		commandList->IASetVertexBuffers(0, 1, &vbView);

		// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		// SRV�q�[�v�̐ݒ�R�}���h
		ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
		commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);


		// SRV�q�[�v�̐擪�o���h�����擾(SRV�����Ă���͂�)
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		// �X�y�[�X���������Ƃ��摜��؂�ւ���
		if (input->PushKey(DIK_SPACE)) { srvGpuHandle.ptr += incrementSize; }

		//		srvGpuHandle.ptr += incrementSize;

				// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// �C���f�b�N�X�o�b�t�@�r���[�̎w��R�}���h
		commandList->IASetIndexBuffer(&ibView);

		// �S�I�u�W�F�N�g�ɂ��ď���
		for (int i = 0; i < _countof(object3ds); i++)
		{
			DrawObject3D(&object3ds[i], commandList.Get(), vbView, ibView, _countof(indicse));
		}

#pragma endregion

#pragma region 5.���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// �`���Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �\����Ԃ�
		commandList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

		// ���߂̃N���[�Y
		result = commandList->Close();
		assert(SUCCEEDED(result));

		// �R�}���h���X�g�̎��s
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		commandQueue.Get()->ExecuteCommandLists(1, commandLists);

		// ��ʂɕ\������o�b�t�@���t���b�v(�\�ʂ̓���ւ�)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		// �R�}���h�̎��s������҂�
		commandQueue->Signal(fence.Get(), ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		// �L���[���N���A
		result = cmdAllocator->Reset();
		assert(SUCCEEDED(result));
		// �ĂуR�}���h���X�g�����߂鏀��
		result = commandList->Reset(cmdAllocator.Get(), nullptr);
		assert(SUCCEEDED(result));

#pragma endregion
	}
	// ���͉��
	delete input;
	input = nullptr;
	// �E�B���h�E�N���X�̉���
	winApp->Finalize();
	// windowAPI���
	delete winApp;
	winApp = nullptr;
	// DirectX���
	delete dxCommon;
	dxCommon = nullptr;

	return 0;
}

// 3D�I�u�W�F�N�g������
void InitializeObject3d(Object3d* object, ID3D12Device* device)
{
	HRESULT result;

	// �萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// �萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTransform));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);
	assert(SUCCEEDED(result));

};

// 3D�I�u�W�F�N�g�X�V����
void UpdataObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);
	matTrans = XMMatrixTranslation(
		object->position.x, object->position.y, object->position.z);

	// ���[���h�s��̍���
	object->matWorld = XMMatrixIdentity();	// �ό`�����Z�b�g
	object->matWorld *= matScale;			// ���[���h�s��ɃX�P�[�����O�𔽉f
	object->matWorld *= matRot;				// ���[���h�s��ɉ�]�𔽉f
	object->matWorld *= matTrans;			// ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (object->parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		object->matWorld *= object->parent->matWorld;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	object->constMapTransform->mat = object->matWorld * matView * matProjection;
}

// 3D�I�u�W�F�N�g�`�揈��
void DrawObject3D(
	Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {
	// ���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);

	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&ibView);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

	// �`��R�}���h
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0); // �S�Ă̒��_���g���ĕ`��
};