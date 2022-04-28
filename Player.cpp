﻿#include "Player.h"
#include "Camera.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"

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

	//コライダーの追加
	float radius = 0.0f;
	float radius2 = 5.0f;
	//球の当たり判定
	player->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0.0 }), radius2));
	player->collider->tag = CollisionTag::TagPlayer;
}

void Player::Update(Camera camera)
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
		//jumppos.y = graundheight;
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


#pragma region 攻撃処理
	if (input->PressKey(DIK_SPACE) && attacktime == 0)
	{
		attack = true;
		attacktime += 1;
		attacktorota = { 0.0f,0.0f,0.0f };
		if (headFlag == true)
		{
			BeamAttack();
		}
	}
	if (attacktime > 0)
	{
		if (attacktime < 30)
		{
			attacktime++;
			attacktorota += Vector3(0.0f, 5.0f, 0.0f);
			if (input->PressKey(DIK_W) || input->PressKey(DIK_S) ||
				input->PressKey(DIK_A) || input->PressKey(DIK_D)) {
				player->SetRotation(player->GetRotation() + attacktorota);
			}
			else {
				player->SetRotation(player->GetRotation() + Vector3(0.0f, 5.0f, 0.0f));
			}			
		}
		else if (attacktime >= 30 && attacktime < 59)
		{
			attacktime++;
			attacktorota += Vector3(0.0f, -5.0f, 0.0f);
			if (input->PressKey(DIK_W) || input->PressKey(DIK_S) ||
				input->PressKey(DIK_A) || input->PressKey(DIK_D)) {
				player->SetRotation(player->GetRotation() + attacktorota);
			}
			else {
				player->SetRotation(player->GetRotation() + Vector3(0.0f, -5.0f, 0.0f));
			}
		}
		else
		{
			//player->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
			attack = false;
			attacktime = 0;
		}
	}
#pragma endregion

	player->Update();
}

void Player::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	player->Draw();
	ModelDraw::PostDraw();
}


void Player::BeamAttack() {
	//説明変数
	const float shotSpeed = 10.0f;
	const float timeOver = 0.0f;
	const float initCharge = 30.0f;
	const float initAttack = 100.0f;
	const float initCoolTime = 100.0f;

	//攻撃用メンバ変数
	if (attack == false) {				
		//chargeTime = initCharge;
		//attackTime = initAttack;
	}
	attack = true;

	//攻撃用ローカル変数
	/*Vector3 direction = oldBossPos - oldPlayerPos;
	direction.Normalize();*/

	//ボスの正面から少し前を求める
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(player->GetRotation().y)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	matRot = XMMatrixRotationY((XMConvertToRadians(player->GetRotation().y)));

	XMVECTOR bossFront = /*oldBossPos +*/ movement * XMVECTOR{ 50, 50, 50 };

	//振動
	/*if (chargeTime >= timeOver) {
		if (chargeTime == initCharge) {
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Charge], Audio::not, 0.2f);
		}
		chargeTime -= 1.0f;
		bulletPos = bossFront;
		bullet->SetPos(bulletPos);
		shakePosX = oldBossPos.x + rand() % 4 - 2;
		shakePosZ = oldBossPos.z + rand() % 4 - 2;
		boss->SetPos(Vector3(shakePosX, oldBossPos.y, shakePosZ));
	}*/
	/*if (chargeTime <= timeOver && attackTime >= timeOver) {
		if (attackTime == initAttack) {
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Shot], Audio::not, 0.5f);
		}
		attackTime -= 1.0f;
		bulletPos -= direction * shotSpeed;
		bullet->SetPos(bulletPos);
	}*/
	if (attacktime <= timeOver) {
	/*	chargeTime = initCharge;
		coolTime = initAttack;*/
		//boss->SetPos(oldBossPos);
		//attacktime = initAttack;
		//attackType = NONE;
		attack = false;
	}
}