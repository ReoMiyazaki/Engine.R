#include "Input.h"
#include <cassert>
//#include <wrl.h>
//using namespace Microsoft::WRL;
//#define DIRECTINPUT_VERSION	0x0800	// DirectInput�̃o�[�W����
//#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

bool Input::PushKey(BYTE KeyNumber)
{
	// �w��L�[�������Ă����true��Ԃ�
	if (key[KeyNumber]) { return true; };
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	if (key[keyNumber] != keyPre[keyNumber]) { return true; };
	return false;
}

void Input::Initialize(WinApp* winApp)
{
	this->winApp_ = winApp;

	HRESULT result;

	// DirectInput�̃C���X�^���X����
	result = DirectInput8Create(winApp_->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	// �W���`��
	assert(SUCCEEDED(result));
	// �r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	HRESULT result;

	// �O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));
	// �L�[�{�[�h���̎擾�J�n
	result = keyboard->Acquire();
	// �S�L�[�̓��͏�Ԃ��擾����
	result = keyboard->GetDeviceState(sizeof(key), key);
}
