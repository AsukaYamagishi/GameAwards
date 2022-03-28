#include "WindowsAPI.h"


WNDCLASSEX WindowsAPI::w;
HWND WindowsAPI::hwnd;
MSG WindowsAPI::msg;
const int WindowsAPI::window_width = 1280;
const int WindowsAPI::window_height = 720;


HINSTANCE WindowsAPI::GetInstance()
{
	return WindowsAPI::w.hInstance;
}

LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージで分岐
	switch (msg)
	{
		case WM_DESTROY: //ウィンドウが破棄された
			PostQuitMessage(0); //OSに対して、アプリの終了を伝える
			return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); //標準の処理を行う
}

bool WindowsAPI::WindowCreate()
{
	//ウィンドウクラスの設定
	WindowsAPI::w.cbSize = sizeof(WNDCLASSEX);
	WindowsAPI::w.lpfnWndProc = (WNDPROC)WindowProc; //ウィンドウプロシージャを設定
	WindowsAPI::w.lpszClassName = L"WindowClassName"; //ウィンドウクラス名
	WindowsAPI::w.hInstance = GetModuleHandle(nullptr); //ウィンドウハンドル
	WindowsAPI::w.hCursor = LoadCursor(NULL, IDC_ARROW); //カーソル指定

	//ウィンドウクラスをOSに登録
	RegisterClassEx(&WindowsAPI::w);
	//ウィンドウサイズ｛x,y,width,height｝
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); //自動でサイズ補正

	//ウィンドウオブジェクトの生成
	WindowsAPI::hwnd = CreateWindow(WindowsAPI::w.lpszClassName, //クラス名
							 L"ゲーム大賞", //タイトルバーの文字
							 WS_OVERLAPPEDWINDOW, //標準的なウィンドウスタイル
							 CW_USEDEFAULT, //表示Ｘ座標（OS依存）
							 CW_USEDEFAULT, //表示Y座標（OS依存）
							 wrc.right - wrc.left, //ウィンドウ横幅
							 wrc.bottom - wrc.top, //ウィンドウ縦幅
							 nullptr, //親ウィンドウハンドル
							 nullptr, //メニューハンドル
							 WindowsAPI::w.hInstance, //呼び出しアプリケーション
							 nullptr); //オプション

	//ウィンドウ表示
	ShowWindow(WindowsAPI::hwnd, SW_SHOW);

	return true;
}

bool WindowsAPI::WindowClassRelease()
{
	UnregisterClass(WindowsAPI::w.lpszClassName, WindowsAPI::w.hInstance);
	return true;
}

bool WindowsAPI::WindowMassage()
{
	//メッセージがあるか
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); //キー入力メッセージの処理
		DispatchMessage(&msg); //プロシージャにメッセージを送る
	}

	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	return true;
}
