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
	end = new EndGame();
	//end->Init(dxCommon, input, audio);
	game = new GameScene();
	game->Init(dxCommon, input, audio);

	//�Q�[���J�n�V�[���̐ݒ�
	sceneNo = gameScene;
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�
	if (input->PressKeyTrigger(DIK_SPACE) && sceneNo == titleScene)
	{
		game->Init(dxCommon, input, audio);
		sceneNo = gameScene;
	}
	else if (game->gameEndFlag == true && sceneNo == gameScene)
	{
		end->Init(dxCommon, input, audio);
		sceneNo = endScene;
	}
	else if (input->PressKeyTrigger(DIK_SPACE) && sceneNo == endScene)
	{
		title->Init(dxCommon, input, audio);
		sceneNo = titleScene;
	}
	else if (input->PressKeyTrigger(DIK_RETURN) && sceneNo == endScene)
	{
		game->Init(dxCommon, input, audio);
		sceneNo = gameScene;
	}

#pragma region �V�[���A�b�v�f�[�g
	
	if (sceneNo == titleScene) {
		title->Update();
	}
	else if (sceneNo == gameScene){
		game->Update();
	}
	else if (sceneNo == endScene) {
		end->Update();
	}
#pragma endregion

}

void SceneManager::Draw()
{
	//�I���V�[���`��
	if (sceneNo == titleScene) {
		title->Draw();
	}
	else if (sceneNo == gameScene)
	{
		game->Draw();
	}
	else if (sceneNo == endScene) {
		end->Draw();
	}
}
