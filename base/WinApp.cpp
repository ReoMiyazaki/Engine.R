#include "WinApp.h"
#include <windows.h>

LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:		// ウィンドウが破棄された
		PostQuitMessage(0);	// OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::Initialize()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;		// ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame";			// ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);		// ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);	// カーソル設定
	// ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);
	// ウィンドウサイズ { X座標, Y座標, 横幅, 縦幅 }
	RECT wrc = { 0,0,window_width,window_hright };
	// 自動でサイズ補正
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,	// クラス名
		L"DirectXGame",			// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// 標準的なウィンドウスタイル
		CW_USEDEFAULT,			// 表示X座標(OSに任せる)
		CW_USEDEFAULT,			// 表示Y座標(OSに任せる)
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		w.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);				// オプション
	// ウィンドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);
}

void WinApp::Update()
{
}

void WinApp::Finalize()
{
	// ウィンドウズクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WinApp::ProcessMessage()
{
	MSG msg{};	// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}
