#pragma once
#include <windows.h>

class WinApp
{
public: // 静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// ウィンドウのサイズ設定
	static const int window_width = 1280;	// 横幅
	static const int window_hright = 720;	// 縦幅

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd; }

	HINSTANCE GetInstance() { return w.hInstance; }

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了かどうか</returns>
	bool ProcessMessage();

private:
	HWND hwnd = nullptr;	// ウィンドウハンドル
	WNDCLASSEX w{};	// ウィンドウクラスの設定
};