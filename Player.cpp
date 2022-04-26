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

void Player::Update(Camera camera)
{

#pragma region	移動処理
	/*if (input->PressKey(DIK_W)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, 0.0f, move));
	}
	if ( input->PressKey(DIK_S)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, 0.0f, -move));
	}*/
	//if (input->PressKey(DIK_A)) {
	//	/*player->SetPos(player->GetPos() + Vector3(-move, 0.0f, 0.0f));*/
	//	player->SetRotation(player->GetRotation() + Vector3(0.0f, -move, 0.0f));
	//}
	//if (input->PressKey(DIK_D)) {
	//	/*player->SetPos(player->GetPos() + Vector3(+move, 0.0f, 0.0f));*/
	//	player->SetRotation(player->GetRotation() + Vector3(0.0f, move, 0.0f));
	//}

	//デバッグ移動
	if (input->PressKey(DIK_E)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, move, 0.0f));
	}
	if (input->PressKey(DIK_Q)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, -move, 0.0f));
	}

	XMVECTOR forvardvec = {};

	bool isinput = false;

	if (input->PressKey(DIK_W)) {
		forvardvec.m128_f32[2] += 1;
		isinput = true;
	}
	else if (input->PressKey(DIK_S)) {
		forvardvec.m128_f32[2] -= 1;
		isinput = true;
	}
	if (input->PressKey(DIK_A)) {
		forvardvec.m128_f32[0] -= 1;
		isinput = true;
	}
	if (input->PressKey(DIK_D)) {
		forvardvec.m128_f32[0] += 1;
		isinput = true;
	}


	XMVECTOR playermatrot = { forvardvec };
	//回転行列をかける
	playermatrot = XMVector3Normalize(playermatrot);
	playermatrot = XMVector3Transform(playermatrot, camera.matrot);
	//正規化する
	playermatrot = XMVector3Normalize(playermatrot);


	XMFLOAT3 rote = player->GetRotation();
	XMFLOAT3 pos = player->GetPos();
	forvardvec = XMVector3TransformNormal(forvardvec, camera.matrot);

	float speed = 1.0f;

	Vector3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };

	player->SetPos(player->GetPos() + move);
	if (isinput) {
		float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
		player->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
	}
	


#pragma endregion

	player->Update();

#pragma region 攻撃処理
	if (input->PressKey(DIK_SPACE) && attacktime == 0)
	{

		attack = true;
		attacktime += 1;
		attacktorota = { 0.0f,0.0f,0.0f };
	}
	if (attacktime > 0)
	{
		Vector3 rota = player->GetRotation();
		if (attacktime < 30)
		{
			attacktime++;
			player->SetRotation(player->GetRotation() + Vector3(0.0f, 5.0f, 0.0f));
			attacktorota += Vector3(0.0f, 5.0f, 0.0f);
		}
		else if (attacktime >= 30 && attacktime < 59)
		{
			attacktime++;
			player->SetRotation(player->GetRotation() + Vector3(0.0f, -5.0f, 0.0f));
			attacktorota += Vector3(0.0f, -5.0f, 0.0f);
		}
		else
		{
			//player->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
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