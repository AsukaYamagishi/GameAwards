#include "KeyboardInput.h"

//インクルード関係
#define DIRECTINPUT_VERSION 0x0800 //DirectInputバージョン指定
#include<dinput.h>

//キーボードデバイスの生成
IDirectInputDevice8* devkeyboard = nullptr;

//キーボード情報の利用
int KeyboardInput::pressKey[256] = { 0 };
int KeyboardInput::releaseKey[256] = { 0 };

KeyboardInput::KeyboardInput()
{
}

KeyboardInput::~KeyboardInput()
{
}

//初期化
void KeyboardInput::Init(HWND hwnd)
{
	HRESULT result;

	//DirectInputオブジェクトの生成
	IDirectInput8* dinput = nullptr;
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	//キーボードデバイスの生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); //デバイスの種類による

	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

}

//毎フレーム更新
void KeyboardInput::Update()
{
	HRESULT result;
	BYTE key[256] = { 0 };

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();

	//全キーの入力状態を取得する
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


//情報の利用
//キーを押した瞬間
bool KeyboardInput::PressKeyTrigger(int key)
{
	return pressKey[key] == 1;
}
//キーを離した瞬間
bool KeyboardInput::ReleaseKeyTrigger(int key)
{
	return releaseKey[key] == 1;
}
//キーを押している間
bool KeyboardInput::PressKey(int key)
{
	return pressKey[key] > 0;
}
//キーを離している間
bool KeyboardInput::ReleaseKey(int key)
{
	return releaseKey[key] > 0;
}

KeyboardInput* KeyboardInput::GetInstance()
{
	static KeyboardInput instance;
	return &instance;
}
