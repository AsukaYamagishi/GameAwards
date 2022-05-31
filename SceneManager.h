#pragma once
#include "GameScene.h"
#include "Title.h"
#include "EndGame.h"

class SceneManager
{
#pragma region �ϐ�
private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio* audio = nullptr;
	GameScene* game = nullptr;
	Title* title = nullptr;
	EndGame* end = nullptr;
	Audio::SoundData soundData[2];


	Sprite* black = nullptr;
	Sprite* enemy = nullptr;
	Sprite* enemy_White = nullptr;

	//�V�[���J�ڔԍ�
	enum SceneNo {
		titleScene,
		gameScene,
		endScene
	};
	SceneNo sceneNo;

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
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();
	//������
	void Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();
#pragma endregion
private:
	bool result=false;
	float alpha;
	bool titleGameFlag;
	bool gameEndFlag;

	//�A�j���[�V����
	float enemyTimer;
	float enemyMaxTimer;
	float i;
	float count;
};

