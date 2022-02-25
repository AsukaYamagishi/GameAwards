#include "Player.h"
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

void Player::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	player->SetScale(Vector3(10,10,10));
}

void Player::Update()
{
	player->Update();

#pragma region	プレイヤー移動
	if (input->PressKey(DIK_UP)|| input->PressKey(DIK_W)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, +move, 0.0f));
	}
	if (input->PressKey(DIK_DOWN)|| input->PressKey(DIK_S)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, -move, 0.0f));
	}
	if (input->PressKey(DIK_LEFT)|| input->PressKey(DIK_A)) {
		player->SetPos(player->GetPos() + Vector3(-move, 0.0f, 0.0f));
	}
	if (input->PressKey(DIK_RIGHT)|| input->PressKey(DIK_D)) {
		player->SetPos(player->GetPos() + Vector3(+move, 0.0f, 0.0f));
	}
#pragma endregion

}

void Player::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	player->Draw();
	ModelDraw::PostDraw();
	
}

