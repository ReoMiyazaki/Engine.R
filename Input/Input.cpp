#include "Input.h"
#include <cassert>
//#include <wrl.h>
//using namespace Microsoft::WRL;
//#define DIRECTINPUT_VERSION	0x0800	// DirectInputのバージョン
//#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

bool Input::PushKey(BYTE KeyNumber)
{
	// 指定キーを押していればtrueを返す
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

	// DirectInputのインスタンス生成
	result = DirectInput8Create(winApp_->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	// 標準形式
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(winApp_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	HRESULT result;

	// 前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	// キーボード情報の取得開始
	result = keyboard->Acquire();
	// 全キーの入力状態を取得する
	result = keyboard->GetDeviceState(sizeof(key), key);
}
