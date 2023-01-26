#pragma once
#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION	0x0800	// DirectInputのバージョン
#include <dinput.h>

// 入力
class Input
{
public:
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private: // メンバ変数
	// キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> keyboard;
};