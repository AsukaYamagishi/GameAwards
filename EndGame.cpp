#include "EndGame.h"
#include <cassert>

using namespace DirectX;

EndGame::EndGame()
{
}

EndGame::~EndGame()
{
	safe_delete(spriteBG);
	//safe_delete(sprite1);
}

void EndGame::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(keyInput);
	assert(padInput);
	assert(audio);

	this->dxCommon = dxCommon;
	this->keyInput = keyInput;
	this->padInput = padInput;
	this->audio = audio;


#pragma region sprite�֌W
	// �e�N�X�`���ǂݍ���(�P�Ԃɂ���Ƃ悭�킩���G���[�N����)
	if (!Sprite::LoadTexture(2, L"Resources/sprite/clear.png")) {
		assert(0);
		return;
	}
	Sprite::LoadTexture(3, L"Resources/sprite/failure.png");
	Sprite::LoadTexture(30, L"Resources/sprite/number.png");
	//// �w�i�X�v���C�g����
	spriteBG = Sprite::CreateSprite(2, { 0.0f,0.0f });
	spriteBG->SetSize({ 1280, 720 });
	spriteBG->Update();

	sprite1 = Sprite::CreateSprite(3, { 0.0f,0.0f });
	sprite1->SetSize({ 1280, 720 });
	sprite1->Update();

	number = Sprite::CreateSprite(30, { 0.0f,0.0f });
	number->Update();
	
#pragma endregion
}

void EndGame::Update()
{
	number->SetSize({ 128, 128 });
	number->SetTextureRect({ 0,0 }, { 32, 32 });
	number->Update();
}

void EndGame::Draw(bool winjude)
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	if (winjude)
	{
		spriteBG->Draw();
	}
	else
	{
		sprite1->Draw();
	}

	number->Draw();
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

	// 3D�I�u�W�F�N�g�`��㏈��
	//Object3D::PostDraw();
#pragma endregion

#pragma region 3D���f���`��
// 3D���f���`��O����
	//ModelObj::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	//model->Draw();

	// 3D�I�u�W�F�N�g�`��㏈��
	//ModelObj::PostDraw();
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// �����ɑO�i�X�v���C�g�̕`�揈����ǉ��ł���

	//AL3 1-2�ǉ�
	//sprite1->Draw();
	//sprite2->Draw();
	
	

	// �f�o�b�O�e�L�X�g�̕`��
	//debugText.DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
