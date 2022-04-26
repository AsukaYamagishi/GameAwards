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

void Player::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
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

#pragma region	移動処理
	/*if (input->PressKey(DIK_W)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, 0.0f, move));
	}
	if ( input->PressKey(DIK_S)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, 0.0f, -move));
	}*/
	if (input->PressKey(DIK_A)) {
		/*player->SetPos(player->GetPos() + Vector3(-move, 0.0f, 0.0f));*/
		player->SetRotation(player->GetRotation() + Vector3(0.0f, -move, 0.0f));
	}
	if (input->PressKey(DIK_D)) {
		/*player->SetPos(player->GetPos() + Vector3(+move, 0.0f, 0.0f));*/
		player->SetRotation(player->GetRotation() + Vector3(0.0f, move, 0.0f));
	}

	//デバッグ移動
	if (input->PressKey(DIK_E)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, move, 0.0f));
	}
	if (input->PressKey(DIK_Q)) {
		player->SetPos(player->GetPos() + Vector3(0.0f, -move, 0.0f));
	}

	//ジャンプ
	Vector3 jumppos = player->GetPos();
	if (jumppos.y <=graundheight)
	{
		jumpadd = initjumpNum;
		jumppos.y = graundheight;
		jumpflag = false;
	}
	if (input->PressKey(DIK_J)&& jumppos.y <= graundheight) {
		jumpflag = true;
	}
	//ジャンプフラグがあったら加算させて上昇させる
	if (jumpflag == true)
	{
		jumpadd += 0.05f;
		if (jumpadd >= 0)
		{
			jumpadd -= 0.1f;
			jumppos.y += jumpadd;
		}
		else
		{
			jumpflag = false;
			jumpadd = initjumpNum;
		}
	}

	//重力の処理
	if (!jumpflag && jumppos.y > graundheight)
	{
		gravity -= 0.095;
		jumppos.y += gravity;
	}
	if (jumppos.y <= graundheight)
	{
		gravity = 0.0f;
		jumppos.y = graundheight;
	}
	player->SetPos(jumppos);


	XMFLOAT3 rote = player->GetRotation();
	XMFLOAT3 pos = player->GetPos();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	movement = XMVector3TransformNormal(movement, matRot);

	if (input->PressKey(DIK_W)) {
		pos.x += movement.m128_f32[0];
		pos.y += movement.m128_f32[1];
		pos.z += movement.m128_f32[2];
	}
	else if (input->PressKey(DIK_S)) {
		pos.x -= movement.m128_f32[0];
		pos.y -= movement.m128_f32[1];
		pos.z -= movement.m128_f32[2];
	}
#pragma endregion

	player->SetPos(pos);
	player->SetRotation(rote);
	player->Update();
	/*if (input->PressKey(DIK_Z)) {
		player->SetRotation(player->GetRotation() + Vector3(0.0f, 2.0f, 0.0f));
	}
	if (input->PressKey(DIK_C)) {
		player->SetRotation(player->GetRotation() + Vector3(0.0f, -2.0f, 0.0f));
	}*/



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
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	player->Draw();
	ModelDraw::PostDraw();

}