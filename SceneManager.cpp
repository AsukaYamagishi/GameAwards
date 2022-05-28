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
	if (!Sprite::LoadTexture(20, L"Resources/sprite/black.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(21, L"Resources/sprite/enemy.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(22, L"Resources/sprite/enemy_White.png")) {
		assert(0);
		return;
	}
	//// �X�v���C�g����
	black = Sprite::CreateSprite(20, { 0.0f,0.0f });
	enemy = Sprite::CreateSprite(21, { 0,0 });
	enemy_White = Sprite::CreateSprite(22, { 0,0 });
#pragma endregion

	//�V�[���J�ڕϐ�
	titleGameFlag = false;
	gameEndFlag = false;
	alpha = 0;
	enemyTimer = 0;
	enemyMaxTimer = 5;
	i = 0;
	count = 0;
}

void SceneManager::Update()
{
	//�V�[���J��
	black->SetColor({ 0,0,0,alpha });
	black->Update();

	enemy->SetSize({ 128, 128 });
	enemy->SetPosition({ 1100, 580 });
	enemy->SetTextureRect({128 * i, 0}, {128, 128});
	enemy->Update();

	enemy_White->SetSize({ 160, 140 });
	enemy_White->SetPosition({ 1055, 565 });
	enemy_White->SetTextureRect({ 128 * i, 0 }, { 128, 128 });
	enemy_White->Update();

	for (int i = 0; i < 256; i++) {
		if (keyInput->PressKey(i)) {
			title->isKeyboard = true;
			title->isGamePad = false;
		}
	}

	for (int i = 0; i < 32; i++) {
		if (padInput->IsPadButton(i) || padInput->IsPadStick(i + 1, 0.1f)) {
			title->isGamePad = true;
			title->isKeyboard = false;
		}
	}

	//�V�[���؂�ւ�
	if ((keyInput->PressKeyTrigger(DIK_RETURN) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && sceneNo == titleScene && title->weaponFlag == false && title->breakFlag == false)
	{		
		game->Finalize();
		game->Init(dxCommon, keyInput, padInput, audio);
		title->weaponFlag = true;
	}

	if (title->timer >= 100)
	{
		titleGameFlag = true;
		title->timer = 0;
	}

	if (titleGameFlag == true)
	{
		alpha += 0.05;

		if (alpha >= 1)
		{
			//�A�j���[�V����
			enemyTimer++;
			if (enemyTimer >= enemyMaxTimer)
			{
				i++;
				count++;
				enemyTimer = 0;

				if (i >= 6)
				{
					i = 0;
				}

				if (count >= 12)
				{
					sceneNo = gameScene;
					alpha = 0;
					titleGameFlag = false;
					i = 0;
					count = 0;
				}
			}
		}
	}

	if (game->gameFlag == true && sceneNo == gameScene)
	{
		end->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = endScene;
	}

	if ((keyInput->PressKeyTrigger(DIK_RETURN) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && sceneNo == endScene)
	{
		title->Init(dxCommon, keyInput, padInput, audio);
		gameEndFlag = true;
	}

	if (gameEndFlag == true)
	{
		alpha += 0.05;

		if (alpha >= 1)
		{
			//�A�j���[�V����
			enemyTimer++;
			if (enemyTimer >= enemyMaxTimer)
			{
				i++;
				count++;
				enemyTimer = 0;

				if (i >= 6)
				{
					i = 0;
				}

				if (count >= 12)
				{
					sceneNo = titleScene;
					alpha = 0;
					gameEndFlag = false;
					i = 0;
					count = 0;
				}
			}
		}
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
	if (titleGameFlag == true || gameEndFlag == true)
	{
		black->Draw();
		enemy_White->Draw();
		enemy->Draw();
	}

	// �f�o�b�O�e�L�X�g�̕`��
	//debugText.DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
