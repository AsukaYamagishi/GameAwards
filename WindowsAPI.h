#pragma once
#include<Windows.h>

class WindowsAPI
{
public:
	static HINSTANCE GetInstance();
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static bool WindowCreate();
	static bool WindowClassRelease();
	static bool WindowMassage();
	static WNDCLASSEX w;
	static HWND hwnd;
	static const int window_width; //â°ïù
	static const int window_height; //ècïù

private:
	static MSG msg;

};

