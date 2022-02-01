#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Object2D.h"
#include "Object3D.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include "ModelObj.h"

using namespace Microsoft::WRL;
using namespace DirectX;

//�Q�[���V�[��
class Tutorial
{
#pragma region �ϐ�
private: //�ÓI�����o�ϐ�
	

private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	//�Q�[���V�[���p
	Object2D* spriteBG = nullptr;
	Object2D* sprite1 = nullptr;

	//Audio::SoundData soundData[2];
#pragma endregion

#pragma region �֐�
public: //�����o�֐�
	//�R���X�g���N�^
	Tutorial();
	//�f�X�g���N�^
	~Tutorial();
	//������
	void Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();
#pragma endregion
};

