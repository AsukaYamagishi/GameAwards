#include "Player.h"
#include "Camera.h"
#include <time.h>
#include <cassert>

using namespace DirectX;

Player::Player()
{
	player = ModelDraw::Create();
	player->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
}

Player::~Player()
{


}

void Player::Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	player->SetScale(Vector3(1, 1, 1));
	player->SetPos(Vector3(0, 5, 0));
}

void Player::Update()
{
	player->Update();

#pragma region	�v���C���[�ړ�
	if (input->PressKey(DIK_W)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, 0.0f, move));
	}
	if ( input->PressKey(DIK_S)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, 0.0f, -move));
	}
	if ( input->PressKey(DIK_A)) {
		player->SetPos(player->GetPos() + Vector3(-move, 0.0f, 0.0f));
	}
	if (input->PressKey(DIK_D)) {
		player->SetPos(player->GetPos() + Vector3(+move, 0.0f, 0.0f));
	}

	//デバッグ移動
	if (input->PressKey(DIK_E)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, move, 0.0f));
	}
	if (input->PressKey(DIK_Q)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, -move, 0.0f));
	}

	/*if (input->PressKey(DIK_Z)) {
		player->SetRotation(player->GetRotation() + Vector3(0.0f, 2.0f, 0.0f));
	}
	if (input->PressKey(DIK_C)) {
		player->SetRotation(player->GetRotation() + Vector3(0.0f, -2.0f, 0.0f));
	}*/
#pragma endregion


#pragma region �U��
	if (input->PressKey(DIK_SPACE) && attacktime == 0)
	{
		attack = true;
		attacktime += 1;
	}
	if (attacktime > 0)
	{
		Vector3 rota = player->GetRotation();
		attacktime++;
		if (attacktime < 30)
		{
			player->SetRotation(player->GetRotation() + Vector3(0.0f, 5.0f, 0.0f));
		}
		else if (rota.y > 0)
		{
			player->SetRotation(player->GetRotation() + Vector3(0.0f, -5.0f, 0.0f));
		}
		else
		{
			player->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			attack = false;
			attacktime = 0;
		}
	}
#pragma endregion


}

void Player::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	player->Draw();
	ModelDraw::PostDraw();

}