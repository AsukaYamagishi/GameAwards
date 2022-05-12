#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Sprite.h"

using namespace Microsoft::WRL;
using namespace DirectX;

//�Q�[���V�[��
class Title
{
#pragma region �ϐ�
private: //�ÓI�����o�ϐ�
	

private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio* audio = nullptr;
	
	//�Q�[���V�[���p
	Sprite* spriteBG = nullptr;
	Sprite* background = nullptr;

	//���y
	Audio::SoundData soundData;

#pragma endregion

#pragma region �ÓI�����o�֐�

private: //�ÓI�����o�֐�
public: //�ÓI�����o�֐�
#pragma endregion

#pragma region �Q�b�^�[/�Z�b�^�[
	//a
#pragma endregion

#pragma region �����o�֐�
public: //�����o�֐�
	//�R���X�g���N�^
	Title();
	//�f�X�g���N�^
	~Title();
	//������
	void Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();
#pragma endregion
};
