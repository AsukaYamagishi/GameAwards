#pragma once
#include"WindowsAPI.h"

//インクルード関係
#define DIRECTINPUT_VERSION 0x0800 //DirectInputバージョン指定
#include<dinput.h>

class KeyboardInput final
{
private:
	static int pressKey[256];
	static int releaseKey[256];

private: //メンバ関数
	//コンストラクタ
	KeyboardInput();
	//デストラクタ
	~KeyboardInput();

public: //静的メンバ関数
	//初期化
	static void Init(HWND hwnd);
	//毎フレーム更新
	static void Update();
	//情報の利用
	//static void GetHitKeyStateAll(int *key);
	//static bool CheckHitKey(int key);
	//static bool CheckHitKeyTrigger(int key);

//private:
	static bool PressKeyTrigger(int key);
	static bool ReleaseKeyTrigger(int key);
	static bool PressKey(int key);
	static bool ReleaseKey(int key);


public:
	//コピーコンストラクタ無効
	KeyboardInput(const KeyboardInput& obj) = delete;
	//代入演算子無効
	KeyboardInput& operator=(const KeyboardInput& obj) = delete;
	//インスタンス生成
	static KeyboardInput* GetInstance();
};