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
	if (!Sprite::LoadTexture(2, L"Resources/sprite/title.png")) {
		assert(0);
		return;
	}
	if (!Sprite::LoadTexture(3, L"Resources/sprite/title_back.png")) {
		assert(0);
		return;
	}
	// �w�i�X�v���C�g����
	spriteBG = Sprite::CreateSprite(2, { 0.0f,0.0f });
	spriteBG->SetSize({ 1280, 720 });
	spriteBG->Update();

	background = Sprite::CreateSprite(3, { 0.0f,0.0f });
	background->SetSize({ 1280, 720 });
	background->Update();
#pragma endregion


#pragma region 3D���f��Create�E�����ݒ�
#pragma endregion
	
#pragma region ���y���\�[�X�����ݒ�


	soundData = audio->SoundLoadWave("Resources/Sound/BGM/Title.wav");
	//audio->SoundPlayWave(audio->xAudio2.Get(), soundData, Audio::loop, 0.2f);

#pragma endregion
}

void Title::Update()
{

}

void Title::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �w�i�X�v���C�g�`��
	background->Draw();

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
	
#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	// �f�o�b�O�e�L�X�g�̕`��
//debugText.DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
