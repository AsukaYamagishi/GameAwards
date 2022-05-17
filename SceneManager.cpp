#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
{
#pragma region nullptr�`�F�b�N/���
	assert(dxCommon);
	assert(keyInput);
	assert(padInput);
	assert(audio);

	this->dxCommon = dxCommon;
	this->keyInput = keyInput;
	this->padInput = padInput;
	this->audio = audio;
#pragma endregion

	//�e�V�[������/�ŏ��̃V�[��������
	title = new Title();
	title->Init(dxCommon, keyInput, padInput, audio);
	end = new EndGame();
	//end->Init(dxCommon, input, audio);
	game = new GameScene();
	game->Init(dxCommon, keyInput, padInput, audio);
	title->Init(dxCommon, keyInput, padInput, audio);
	//�Q�[���J�n�V�[���̐ݒ�
	sceneNo = titleScene;

#pragma region Sprite�����ݒ�
	// �e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(10, L"Resources/sprite/black.png")) {
		assert(0);
		return;
	}
	//// �X�v���C�g����
	black = Sprite::CreateSprite(10, { 0.0f,0.0f });
	
#pragma endregion

	//�V�[���J�ڕϐ�
	sceneFlag = true;
	alpha = 0;
}

void SceneManager::Update()
{
	//�V�[���J��
	
	black->SetColor({ 1,0,0,0.5 });
	black->Update();

	//�V�[���؂�ւ�
	if ((keyInput->PressKeyTrigger(DIK_RETURN) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && sceneNo == titleScene)
	{		
		game->Finalize();
		game->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = gameScene;
	}
	else if (game->gameEndFlag == true && sceneNo == gameScene)
	{
		end->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = endScene;
	}
	else if ((keyInput->PressKeyTrigger(DIK_RETURN) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && sceneNo == endScene)
	{
		title->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = titleScene;
	}

	//�Q�[���V�[���f�o�b�O�p
	if (keyInput->PressKeyTrigger(DIK_M))
	{
		game->Finalize();
		game->Init(dxCommon, keyInput, padInput, audio);
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
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	//�I���V�[���`��
	if (sceneNo == titleScene) {
		title->Draw();
	}
	else if (sceneNo == gameScene)
	{
		game->Draw();
	}
	else if (sceneNo == endScene) {
		end->Draw(game->winJudeg);
	}

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	//�O�i�X�v���C�g�`��
	black->Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	//debugText.DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
