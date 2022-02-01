#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include <vector>


using namespace Microsoft::WRL;
using namespace DirectX;

//�Q�[���V�[��
class GameScene
{
#pragma region �ϐ�
private: //�ÓI�����o�ϐ�
	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`��
	static const int debugTextTexNum = 0;

private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	//�Q�[���V�[���p
	//���y
	Audio::SoundData soundData[3];

#pragma endregion

#pragma region �ÓI�����o�֐�
	
private: //�ÓI�����o�֐�
public: //�ÓI�����o�֐�
#pragma endregion

#pragma region �Q�b�^�[/�Z�b�^�[
	//a
#pragma endregion

#pragma region �����o�֐�

private: //�����o�֐�
public: //�����o�֐�
	//�R���X�g���N�^
	GameScene();
	//�f�X�g���N�^
	~GameScene();
	//������
	void Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();
#pragma endregion

};

