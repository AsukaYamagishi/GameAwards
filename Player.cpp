#include "Player.h"
#include "Camera.h"
#include "Boss.h"
#include <time.h>
#include <cassert>
#include "SphereCollider.h"

using namespace DirectX;

Player::Player()
{
	player = ModelDraw::Create();
	player->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Player));
	bullet = ModelDraw::Create();
	bullet->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Bullet));
}

Player::~Player()
{


}

void Player::Initialize(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
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

	player->SetScale(Vector3(1, 1, 1));
	player->SetPos(Vector3(0, 5, -100));
	bullet->SetScale(Vector3(10, 10, 10));
	bullet->SetPos(Vector3(10, -100, 10));

	//コライダーの追加
	float radius = 0.0f;
	float radius2 = 10.0f;
	//球の当たり判定
	player->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0.0 }), radius2));
	player->collider->tag = CollisionTag::TagPlayer;

	bullet->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0.0 }), 15));
	bullet->collider->tag = CollisionTag::TagPlayerBullet;
}

void Player::Update(Camera camera, Vector3 bossPos, bool cameraFlag)
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
	//if (input->PressKey(DIK_E)) {
	//	player->SetPos(player->GetPos() + Vector3(0.0f, move, 0.0f));
	//}
	//if (input->PressKey(DIK_Q)) {
	//	player->SetPos(player->GetPos() + Vector3(0.0f, -move, 0.0f));
	//}

	//ジャンプ
	Vector3 jumppos = player->GetPos();
	if (jumppos.y <=graundheight)
	{
		jumpadd = initjumpNum;
		//jumppos.y = graundheight;
		jumpflag = false;
	}
	if ((keyInput->PressKey(DIK_LSHIFT) || padInput->IsPadButtonTrigger(XBOX_INPUT_A)) && jumppos.y <= graundheight) {
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

	if (knockBackFlag == false) {
		if (keyInput->PressKey(DIK_W)) {
			forvardvec.m128_f32[2] += 1;
			isinput = true;
		}
		else if (keyInput->PressKey(DIK_S)) {
			forvardvec.m128_f32[2] -= 1;
			isinput = true;
		}
		if (keyInput->PressKey(DIK_A)) {
			forvardvec.m128_f32[0] -= 1;
			isinput = true;
		}
		if (keyInput->PressKey(DIK_D)) {
			forvardvec.m128_f32[0] += 1;
			isinput = true;
		}

		if (padInput->IsPadStick(INPUT_AXIS_LX, 0.1f) != 0 || padInput->IsPadStick(INPUT_AXIS_LY, 0.1f) != 0)
		{
			forvardvec.m128_f32[0] += 1.0f * (padInput->IsPadStick(INPUT_AXIS_LX, 0.1f) / 1000);
			forvardvec.m128_f32[2] += 1.0f * -(padInput->IsPadStick(INPUT_AXIS_LY, 0.1f) / 1000);

			isinput = true;
		}
	}

	/*if (cameraFlag == true) {
		float angle = atan2f(bossPos.x - player->GetPos().x, bossPos.z - player->GetPos().z);
		if (keyInput->PressKey(DIK_W)) {
			angle;
			Vector3 Vec = Vector3 (cosf(angle), 0, sinf(angle));
			player->SetPos(player->GetPos() + Vec * XMVECTOR{ 2, 2, 2});
		}
		else if (keyInput->PressKey(DIK_S)) {
			angle *= -1;
			Vector3 Vec = Vector3(cosf(angle), 0, sinf(angle));
			player->SetPos(player->GetPos() + Vec * XMVECTOR{ 2, 2, 2 });
		}
		else if (keyInput->PressKey(DIK_A)) {
			angle -= XM_PI / 2.0f;
			Vector3 Vec = Vector3(cosf(angle), 0, sinf(angle));
			player->SetPos(player->GetPos() +  Vec * XMVECTOR{ 2, 2, 2 });
		}
		else if (keyInput->PressKey(DIK_D)) {
			angle += XM_PI / 2.0f;
			Vector3 Vec = Vector3(cosf(angle), 0, sinf(angle));
			player->SetPos(player->GetPos() + Vec *  XMVECTOR{ 2, 2, 2 });
		}
	}*/



	XMVECTOR playermatrot = { forvardvec };
	//回転行列をかける
	playermatrot = XMVector3Normalize(playermatrot);
	playermatrot = XMVector3Transform(playermatrot, camera.matRot);
	//正規化する
	playermatrot = XMVector3Normalize(playermatrot);


	XMFLOAT3 rote = player->GetRotation();
	XMFLOAT3 pos = player->GetPos();
	forvardvec = XMVector3TransformNormal(forvardvec, camera.matRot);

	float speed = 1.0f;

	Vector3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };

	//プレイヤーがエリア外に出ないようにする
	Vector3 genten = { 0.0f, 0.0f, 0.0f };
	float distance = 0.0f;
	float distanceX = genten.x - (player->GetPos().x + move.x);
	float distanceZ = genten.z - (player->GetPos().z + move.z);
	distance = sqrtf((distanceX* distanceX) + (distanceZ * distanceZ));
	if (distance > 960.0f) { move = { 0.0f,0.0f,0.0f }; }

	player->SetPos(player->GetPos() + move);
	if (isinput) {
		float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
		player->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
	}
	


#pragma endregion


#pragma region 攻撃処理
	if ((keyInput->PressKey(DIK_SPACE) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && attacktime == 0 && knockBackFlag == false)
	{
		attack = true;
		attacktime += 1;
		attacktorota = { 0.0f,0.0f,0.0f };
		oldRota = player->GetRotation();
	}
	if (attacktime > 0)
	{
		if (headFlag == true)
		{
			BeamAttack();
		}

		if (attacktime < 30)
		{
			attacktime++;
			attacktorota += Vector3(0.0f, 5.0f, 0.0f);
			if (keyInput->PressKey(DIK_W) || keyInput->PressKey(DIK_S) ||
				keyInput->PressKey(DIK_A) || keyInput->PressKey(DIK_D) ||
				padInput->IsPadStick(INPUT_AXIS_LX, 0.1f) != 0 || 
				padInput->IsPadStick(INPUT_AXIS_LY, 0.1f) != 0) {
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
			if (keyInput->PressKey(DIK_W) || keyInput->PressKey(DIK_S) ||
				keyInput->PressKey(DIK_A) || keyInput->PressKey(DIK_D) ||
				padInput->IsPadStick(INPUT_AXIS_LX, 0.1f) != 0 || 
				padInput->IsPadStick(INPUT_AXIS_LY, 0.1f) != 0) {
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
	if (damageCoolTime > 0) {
		damageCoolTime--;
	}

	player->Update();
	bullet->Update();
}

void Player::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	player->Draw();
	bullet->Draw();
	if (attack == true && headFlag == true) {
		
	}
	ModelDraw::PostDraw();
}

void Player::HitDamege() {
	if (damageCoolTime <= 0) {
		hp -= 1;
		damageCoolTime = 100.0f;
		KnockBack();
	}
}

void Player::KnockBack() {
	float xSpeed = 2.0f;
	float ySpeed = 2.0f;
	float jumpPower;

	if (knockBackFlag == false) {
		oldPlayerPos = player->GetPos();
		knockBackFlag = true;
		jumpPower = 5.0f;
	}

	//プレイヤーの正面から少し前を求める
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(player->GetRotation().y)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	matRot = XMMatrixRotationY((XMConvertToRadians(player->GetRotation().y)));

	XMVECTOR playerFront = oldPlayerPos + movement * XMVECTOR{ 50, 50, 50 };
	
	//プレイヤーの少し前からプレイヤーへのベクトルを求める
	Vector3 knockBackVector = oldPlayerPos - playerFront;
	Vector3 playerPos = player->GetPos();
	//プレイヤーを後ろに下げる
	playerPos.x -= xSpeed;
	playerPos.z -= xSpeed;

	Vector3 nowPos = playerPos - (knockBackVector * xSpeed);

	if (nowPos == playerPos )
	{
		xSpeed = 0;
	}

	//Y軸の処理
	jumpPower -= 0.5f;

	playerPos.y += jumpPower;

	player->SetPos(playerPos);
	knockBackFlag = false;
}



void Player::BeamAttack() {
	//説明変数
	const float shotSpeed = 10.0f;

	//ボスの正面から少し前を求める
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(oldRota.y)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ 1, 1, 1 };
	matRot = XMMatrixRotationY((XMConvertToRadians(oldRota.y)));

	playerFront = player->GetPos() + movement * XMVECTOR{ 50, 50, 50 };

	if (attacktime == 1) {
		bulletPos = playerFront;
	}

	//攻撃用ローカル変数
	Vector3 direction = player->GetPos() - playerFront;
	direction.Normalize();

	bulletPos -= direction * shotSpeed;
	bullet->SetPos(bulletPos);
}