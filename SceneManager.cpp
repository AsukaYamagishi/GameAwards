#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
#pragma region nullptr�`�F�b�N/���
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
#pragma endregion

	//�e�V�[������/�ŏ��̃V�[��������
	title = new Title();
	title->Init(dxCommon, input, audio);
	tutorial = new Tutorial();
	game = new GameScene();

	//�Q�[���J�n�V�[���̐ݒ�
	sceneNo = titleScene;
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�
	if (input->PressKeyTrigger(DIK_SPACE) && sceneNo == titleScene)
	{
		tutorial->Init(dxCommon, input, audio);
		sceneNo = tutorialScene;
	}
	else if (input->PressKeyTrigger(DIK_SPACE) && sceneNo == tutorialScene)
	{
		game->Init(dxCommon, input, audio);
		sceneNo = gameScene;
	}

#pragma region �V�[���A�b�v�f�[�g
	
	if (sceneNo == titleScene) {
		title->Update();
	}
	else if (sceneNo == tutorialScene) {
		tutorial->Update();
	}
	else if (sceneNo == gameScene){
		game->Update();
	}
#pragma endregion

}

void SceneManager::Draw()
{
	//�I���V�[���`��
	if (sceneNo == titleScene) {
		title->Draw();
	}
	else if (sceneNo == tutorialScene) {
		tutorial->Draw();
	}
	else if (sceneNo == gameScene)
	{
		game->Draw();
	}
}
