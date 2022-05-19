#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Sprite.h"
#include "OBJObject.h"
#include "ModelDraw.h"
#include "ModelManager.h"

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
	Sprite* break_background = nullptr;

	ModelDraw* weapon;
	ModelDraw* rubble[6][6];
	bool weaponFlag;
	float speed[6][6];
	float rand_number[6][6];
	float timer;
	float weaponTimer;
	float rotZ;
	float posY;

	//���y
	Audio::SoundData soundData;

public:
	bool breakFlag;
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
