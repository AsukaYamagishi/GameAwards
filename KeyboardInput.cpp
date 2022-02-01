#include "KeyboardInput.h"

//�C���N���[�h�֌W
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�o�[�W�����w��
#include<dinput.h>

//�L�[�{�[�h�f�o�C�X�̐���
IDirectInputDevice8* devkeyboard = nullptr;

//�L�[�{�[�h���̗��p
int KeyboardInput::pressKey[256] = { 0 };
int KeyboardInput::releaseKey[256] = { 0 };

KeyboardInput::KeyboardInput()
{
}

KeyboardInput::~KeyboardInput()
{
}

//������
void KeyboardInput::Init(HWND hwnd)
{
	HRESULT result;

	//DirectInput�I�u�W�F�N�g�̐���
	IDirectInput8* dinput = nullptr;
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	//�L�[�{�[�h�f�o�C�X�̐���
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); //�f�o�C�X�̎�ނɂ��

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

}

//���t���[���X�V
void KeyboardInput::Update()
{
	HRESULT result;
	BYTE key[256] = { 0 };

	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();

	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(key), key);

	for (int i = 0; i < 256; i++)
	{
		if (key[i])
		{
			if (releaseKey[i] != 0) {
				releaseKey[i] = 0;
			}

 			pressKey[i] += 1;
		}
		else
		{
			if (pressKey[i] != 0) {
				pressKey[i] = 0;
			}

			releaseKey[i] += 1;
		}
	}

 }


//���̗��p
//�L�[���������u��
bool KeyboardInput::PressKeyTrigger(int key)
{
	return pressKey[key] == 1;
}
//�L�[�𗣂����u��
bool KeyboardInput::ReleaseKeyTrigger(int key)
{
	return releaseKey[key] == 1;
}
//�L�[�������Ă����
bool KeyboardInput::PressKey(int key)
{
	return pressKey[key] > 0;
}
//�L�[�𗣂��Ă����
bool KeyboardInput::ReleaseKey(int key)
{
	return releaseKey[key] > 0;
}

KeyboardInput* KeyboardInput::GetInstance()
{
	static KeyboardInput instance;
	return &instance;
}
