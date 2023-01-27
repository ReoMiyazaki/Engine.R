#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION	0x0800	// DirectInput�̃o�[�W����
#include <dinput.h>
#include "WinApp.h"

// ����
class Input
{
public:
	// namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name="KeyNumber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>�L�[��������Ă��邩</returns>
	bool PushKey(BYTE KeyNumber);

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name="keyNuMber">�L�[�ԍ�(DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(BYTE keyNumber);

public: // �����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(WinApp* winApp);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

private: // �����o�ϐ�
	// DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> directInput = nullptr;
	// �L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> keyboard;
	// �S�L�[�̓��͏�Ԃ��擾����
	BYTE key[256] = {};
	// �O��L�[�̏��
	BYTE keyPre[256] = {};
	// WindowsAPi
	WinApp* winApp_ = nullptr;
};