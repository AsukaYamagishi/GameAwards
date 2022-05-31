#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "Object3D.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "ModelDraw.h"

using namespace Microsoft::WRL;
using namespace DirectX;

//�Q�[���V�[��
class EndGame
{
#pragma region �ϐ�
private: //�ÓI�����o�ϐ�
	

private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	//�Q�[���V�[���p
	Sprite* spriteBG = nullptr;
	Sprite* sprite1 = nullptr;
	Sprite* number = nullptr;

	//Audio::SoundData soundData[2];
#pragma endregion

#pragma region �֐�
public: //�����o�֐�
	//�R���X�g���N�^
	EndGame();
	//�f�X�g���N�^
	~EndGame();
	//������
	void Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw(bool winjude);
#pragma endregion
};

