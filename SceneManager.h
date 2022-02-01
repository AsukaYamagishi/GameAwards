#pragma once
#include "GameScene.h"
#include "Title.h"
#include "Tutorial.h"

class SceneManager
{
#pragma region �ϐ�
private: //�����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;
	GameScene* game = nullptr;
	Title* title = nullptr;
	Tutorial* tutorial = nullptr;

	//�V�[���J�ڔԍ�
	enum SceneNo {
		titleScene,
		tutorialScene,
		gameScene
	};
	SceneNo sceneNo = titleScene;

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
	void Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();
#pragma endregion
};

