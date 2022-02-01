#pragma once
#include<Windows.h>

//�C���N���[�h�֌W
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�o�[�W�����w��
#include<dinput.h>

class KeyboardInput final
{
private:
	static int pressKey[256];
	static int releaseKey[256];

private: //�����o�֐�
	//�R���X�g���N�^
	KeyboardInput();
	//�f�X�g���N�^
	~KeyboardInput();

public: //�ÓI�����o�֐�
	//������
	static void Init(HWND hwnd);
	//���t���[���X�V
	static void Update();
	//���̗��p
	//static void GetHitKeyStateAll(int *key);
	//static bool CheckHitKey(int key);
	//static bool CheckHitKeyTrigger(int key);

//private:
	static bool PressKeyTrigger(int key);
	static bool ReleaseKeyTrigger(int key);
	static bool PressKey(int key);
	static bool ReleaseKey(int key);


public:
	//�R�s�[�R���X�g���N�^����
	KeyboardInput(const KeyboardInput& obj) = delete;
	//������Z�q����
	KeyboardInput& operator=(const KeyboardInput& obj) = delete;
	//�C���X�^���X����
	static KeyboardInput* GetInstance();
};