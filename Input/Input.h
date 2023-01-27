#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION	0x0800	// DirectInputのバージョン
#include <dinput.h>
#include "WinApp.h"

// 入力
class Input
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="KeyNumber">キー番号(DIK_0 等)</param>
	/// <returns>キーが押されているか</returns>
	bool PushKey(BYTE KeyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNuMber">キー番号(DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // メンバ変数
	// DirectInputのインスタンス生成
	ComPtr<IDirectInput8> directInput = nullptr;
	// キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard;
	// 全キーの入力状態を取得する
	BYTE key[256] = {};
	// 前回キーの状態
	BYTE keyPre[256] = {};
	// WindowsAPi
	WinApp* winApp_ = nullptr;
};