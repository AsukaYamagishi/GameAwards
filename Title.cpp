#include "Title.h"
#include <cassert>

using namespace DirectX;

Title::Title()
{
}

Title::~Title()
{
	//safe_delete�͂����ōs��
}

void Title::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
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

#pragma region Sprite�����ݒ�
	// �e�N�X�`���ǂݍ���(�P�Ԃɂ���Ƃ悭�킩���G���[�N����)
	if (!Sprite::LoadTexture(11, L"Resources/sprite/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(12, L"Resources/sprite/title_back.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(13, L"Resources/sprite/title_break.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(14, L"Resources/sprite/press_B.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(15, L"Resources/sprite/press_ENTER.png")) {
		assert(0);
		return;
	}
	// �w�i�X�v���C�g����
	spriteBG = Sprite::CreateSprite(11, { 0.0f,0.0f });
	spriteBG->SetSize({ 1280, 720 });
	spriteBG->Update();

	background = Sprite::CreateSprite(12, { 0.0f,0.0f });
	background->SetSize({ 1280, 720 });
	background->Update();

	break_background = Sprite::CreateSprite(13, { 0.0f,0.0f });
	break_background->SetSize({ 1280, 720 });
	break_background->Update();

	press_B = Sprite::CreateSprite(14, { 70.0f,0.0f });
	press_B->SetSize({ 1280, 720 });
	press_B->Update();

	press_Enter = Sprite::CreateSprite(15, { 0.0f,0.0f });
	press_Enter->SetSize({ 1280, 720 });
	press_Enter->Update();
#pragma endregion

	
#pragma region ���y���\�[�X�����ݒ�


	/*soundData = audio->SoundLoadWave("Resources/Sound/BGM/Title.wav");
	audio->SoundPlayWave(audio->xAudio2.Get(), soundData, Audio::loop, 0.2f);*/

#pragma endregion
	weaponFlag = false;
	breakFlag = false;
	timer = 0;
	weaponTimer = 0;
	rotZ = 180;
	posY = -280;
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			speed[x][y] = 0;
			rand_number[x][y] = 0;
		}
	}

#pragma region 3D���f��Create�E�����ݒ�
	weapon = ModelDraw::Create();
	weapon->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Weapon));
	weapon->SetPos({ 0,posY,0 });
	weapon->SetScale({ 50,50,50 });
	weapon->SetRotation({ 0,90,rotZ });

	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			rubble[x][y] = ModelDraw::Create();
			rubble[x][y]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::rubble));
			rubble[x][y]->SetPos({ x * 10.0f - 30, 3.0f * y - 40, 0 });
			rubble[x][y]->SetScale({ 10,10,10 });
		}
	}
#pragma endregion

}

void Title::Update()
{
	
	/*if ((keyInput->PressKeyTrigger(DIK_RETURN)))
	{
		weaponFlag = true;
		
	}*/

	if (weaponFlag == true)
	{
		for (int y = 0; y < 6; y++)
		{
			for (int x = 0; x < 6; x++)
			{
				rand_number[x][y] = 0;
				speed[x][y] = 0;
			}
		}
		weaponTimer++;
		rotZ -= weaponTimer * 2;
		posY += weaponTimer * 2;
		weapon->SetRotation({ 0, 90, rotZ });
		weapon->SetPos({ 0,posY,0 });
		if (rotZ <= 0 && posY <= -80)
		{
			weaponFlag = false;
			breakFlag = true;
		}
	}
	
	if (breakFlag == true)
	{
		timer++;

		for (int y = 0; y < 6; y++)
		{
			for (int x = 0; x < 6; x++)
			{
				rand_number[x][y] = rand() % 5 + 1;
				speed[x][y] += rand_number[x][y];
				rubble[x][y]->SetPos({ x * (20.0f + speed[x][y]) - (50 + speed[x][y]), (13.0f + speed[x][y]) * y - (30 + speed[x][y]), -speed[x][y] + 60});
				rubble[x][y]->SetRotation({ speed[x][y] * 2, speed[x][y] * 2, 0});

			}
		}
	}

	if (timer >= 200)
	{
		breakFlag = false;
	}
	
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			rubble[x][y]->Update();
		}
	}
	weapon->Update();
	
}

void Title::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �w�i�X�v���C�g�`��

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	//Object3D::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	//object3d->Draw();

#pragma endregion

#pragma region 3D���f���`��
	ModelDraw::PreDraw(cmdList);
	
	for (int y = 0; y < 6; y++)
	{
		for (int x = 0; x < 6; x++)
		{
			rubble[x][y]->Draw();
		}
	}
	
	// 3D�I�u�W�F�N�g�`��㏈��
	ModelDraw::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �w�i�X�v���C�g�`��
	if (breakFlag == true)
	{
		break_background->Draw();
	}
	else
	{
		background->Draw();
		spriteBG->Draw();
		if (isGamePad) {
			press_B->Draw();
		}
		if (isKeyboard) {
			press_Enter->Draw();
		}
		//press_Enter->Draw();
	}
	
	

	// �f�o�b�O�e�L�X�g�̕`��
//debugText.DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	// �[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D���f���`��
	ModelDraw::PreDraw(cmdList);

	if (weaponFlag == true || breakFlag == true)
	{
		weapon->Draw();
	}
	
	
	// 3D�I�u�W�F�N�g�`��㏈��
	ModelDraw::PostDraw();
#pragma endregion
}
