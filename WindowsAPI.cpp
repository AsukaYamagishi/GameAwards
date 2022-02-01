#include "WindowsAPI.h"


WNDCLASSEX WindowsAPI::w;
HWND WindowsAPI::hwnd;
MSG WindowsAPI::msg;
const int WindowsAPI::window_width = 1280;
const int WindowsAPI::window_height = 720;


LRESULT WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//���b�Z�[�W�ŕ���
	switch (msg)
	{
		case WM_DESTROY: //�E�B���h�E���j�����ꂽ
			PostQuitMessage(0); //OS�ɑ΂��āA�A�v���̏I����`����
			return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); //�W���̏������s��
}

bool WindowsAPI::WindowCreate()
{
	//�E�B���h�E�N���X�̐ݒ�
	WindowsAPI::w.cbSize = sizeof(WNDCLASSEX);
	WindowsAPI::w.lpfnWndProc = (WNDPROC)WindowProc; //�E�B���h�E�v���V�[�W����ݒ�
	WindowsAPI::w.lpszClassName = L"WindowClassName"; //�E�B���h�E�N���X��
	WindowsAPI::w.hInstance = GetModuleHandle(nullptr); //�E�B���h�E�n���h��
	WindowsAPI::w.hCursor = LoadCursor(NULL, IDC_ARROW); //�J�[�\���w��

	//�E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&WindowsAPI::w);
	//�E�B���h�E�T�C�Y�ox,y,width,height�p
	RECT wrc = { 0,0,window_width,window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); //�����ŃT�C�Y�␳

	//�E�B���h�E�I�u�W�F�N�g�̐���
	WindowsAPI::hwnd = CreateWindow(WindowsAPI::w.lpszClassName, //�N���X��
							 L"TitlebarName", //�^�C�g���o�[�̕���
							 WS_OVERLAPPEDWINDOW, //�W���I�ȃE�B���h�E�X�^�C��
							 CW_USEDEFAULT, //�\���w���W�iOS�ˑ��j
							 CW_USEDEFAULT, //�\��Y���W�iOS�ˑ��j
							 wrc.right - wrc.left, //�E�B���h�E����
							 wrc.bottom - wrc.top, //�E�B���h�E�c��
							 nullptr, //�e�E�B���h�E�n���h��
							 nullptr, //���j���[�n���h��
							 WindowsAPI::w.hInstance, //�Ăяo���A�v���P�[�V����
							 nullptr); //�I�v�V����

	//�E�B���h�E�\��
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
	//���b�Z�[�W�����邩
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); //�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); //�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return false;
	}
	return true;
}
