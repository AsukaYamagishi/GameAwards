#include "Player2.h"
#include "Camera.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"

using namespace DirectX;

Player2::Player2()
{
	/*player = ModelDraw::Create();
	player->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));*/
}

Player2::~Player2()
{


}

Player2* Player2::Create(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	Player2* instance = new Player2();
	
	instance->Initialize(dxCommon, input, audio);
	instance->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
	
	return instance;
}



void Player2::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	SetScale(Vector3(1, 1, 1));
	SetPos(Vector3(0, 5, 0));

	//コライダーの追加
	float radius = 0.6f;
	float radius2 = 10.6f;
	//半径分だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0.0 }), radius2));
}

void Player2::Update(Camera camera)
{
	oldattack = attack;
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
		SetPos(GetPos() + Vector3(0.0f, move, 0.0f));
	}
	if (input->PressKey(DIK_Q)) {
		SetPos(GetPos() + Vector3(0.0f, -move, 0.0f));
	}

	//ジャンプ
	Vector3 jumppos = GetPos();
	if (jumppos.y <= graundheight)
	{
		jumpadd = initjumpNum;
		jumppos.y = graundheight;
		jumpflag = false;
	}
	if (input->PressKey(DIK_J) && jumppos.y <= graundheight) {
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
	SetPos(jumppos);

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


	XMFLOAT3 rote = GetRotation();
	XMFLOAT3 pos = GetPos();
	forvardvec = XMVector3TransformNormal(forvardvec, camera.matrot);

	float speed = 1.0f;

	Vector3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };

	SetPos(GetPos() + move);
	if (isinput) {
		float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
		SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
	}



#pragma endregion

	//ModelDraw::Update();

#pragma region 攻撃処理
	if (input->PressKey(DIK_SPACE) && attacktime == 0)
	{

		attack = true;
		attacktime += 1;
		attacktorota = { 0.0f,0.0f,0.0f };
	}
	if (attacktime > 0)
	{
		Vector3 rota = GetRotation();
		if (attacktime < 30)
		{
			attacktime++;
			SetRotation(GetRotation() + Vector3(0.0f, 5.0f, 0.0f));
			attacktorota += Vector3(0.0f, 5.0f, 0.0f);
		}
		else if (attacktime >= 30 && attacktime < 59)
		{
			attacktime++;
			SetRotation(GetRotation() + Vector3(0.0f, -5.0f, 0.0f));
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

void Player2::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	Draw();
	ModelDraw::PostDraw();

}