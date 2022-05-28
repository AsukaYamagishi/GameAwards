#include "Boss.h"
#include"MeshCollider.h"
#include"SphereCollider.h"

using namespace DirectX;

Boss::Boss()
{
	boss = ModelDraw::Create();
	boss->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Plane));

	head = ModelDraw::Create();
	head->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Face));
	body = ModelDraw::Create();
	body->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Body));
	rightarm = ModelDraw::Create();
	rightarm->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Right_arm));
	leftarm = ModelDraw::Create();
	leftarm->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Left_arm));
	rightleg = ModelDraw::Create();
	rightleg->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Leftleg));
	leftleg = ModelDraw::Create();
	leftleg->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Rightleg));
	bullet = ModelDraw::Create();
	bullet->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Bullet));
	shockWave = ModelDraw::Create();
	shockWave->SetModel(ModelManager::GetIns()->GetModel(ModelManager::ShockWave));

	//コライダーの追加
	MeshCollider* headcollider = new MeshCollider;
	MeshCollider* bodycollider = new MeshCollider;
	MeshCollider* rightarmcollider = new MeshCollider;
	MeshCollider* leftarmcollider = new MeshCollider;
	MeshCollider* rightlegcollider = new MeshCollider;
	MeshCollider* leftlegcollider = new MeshCollider;
	//コライダーの追加
	float radius = 0.0f;
	float radius2 = 10.0f;
	//球の当たり判定
	bullet->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0.0 }), radius2));


	head->SetCollider(headcollider);
	headcollider->ConstrucTriangles(head->GetModelInput());

	body->SetCollider(bodycollider);
	bodycollider->ConstrucTriangles(body->GetModelInput());

	rightarm->SetCollider(rightarmcollider);
	rightarmcollider->ConstrucTriangles(rightarm->GetModelInput());

	leftarm->SetCollider(leftarmcollider);
	leftarmcollider->ConstrucTriangles(leftarm->GetModelInput());

	rightleg->SetCollider(rightlegcollider);
	rightlegcollider->ConstrucTriangles(rightleg->GetModelInput());

	leftleg->SetCollider(leftlegcollider);
	leftlegcollider->ConstrucTriangles(leftleg->GetModelInput());

	/*bullet->SetCollider(bulletcollider);
	bulletcollider->ConstrucTriangles(bullet->GetModelInput());*/

	//タグ付け
	headcollider->tag = CollisionTag::TagHead;
	bodycollider->tag = CollisionTag::TagBody;
	rightarmcollider->tag = CollisionTag::TagRightArm;
	leftarmcollider->tag = CollisionTag::TagLeftArm;
	rightlegcollider->tag = CollisionTag::TagRightLeg;
	leftlegcollider->tag = CollisionTag::TagLeftLeg;
	bullet->collider->tag = CollisionTag::TagBullet;
}

Boss::~Boss()
{


}

void Boss::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio, ModelDraw* player)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
	this->player = player;

	head->SetParent(boss);
	body->SetParent(boss);
	rightarm->SetParent(boss);
	leftarm->SetParent(boss);
	rightleg->SetParent(boss);
	leftleg->SetParent(boss);

	rightarm->SetPos(Vector3(0, 11, 32));
	leftarm->SetPos(Vector3(13, -18, 7));
	rightleg->SetPos(Vector3(0, 0, 0));
	leftleg->SetPos(Vector3(0, 0, 0));
	bullet->SetPos(Vector3(0, 0, 0));
	bullet->SetScale(Vector3(20, 20, 20));

	noneArm = false;
	noneLeg = false;

	soundSE[Charge] = audio->SoundLoadWave("Resources/Sound/SE/Charge.wav");
	soundSE[Shot] = audio->SoundLoadWave("Resources/Sound/SE/WeaponAttack_Boss01.wav");
}

void Boss::Update()
{
	//説明変数
	const float moveRange = 20.0f;
	const float beamRange = 60.0f;
	const float pressRange = 0.0f;
	//デバッグ用に0キーを押すとボスの動きが止まる
	if (input->PressKeyTrigger(DIK_0)) {
		if (stopFlag == false) {
			stopFlag = true;
		}
		else {
			stopFlag = false;
		}
	}

	//クールタイムを減算し続ける
	coolTime -= 1.0f;
	//プレイヤーの一定距離まで移動する
	if (hp > 0) {
		if (RangeJudge(moveRange) && stopFlag == false && attackFlag == false && noneArm == false) {
			Move();
		}
		//プレイヤーの方を見続ける
		if (stopFlag == false && attackFlag == false) {
			Direction();
		}
		//攻撃
		if (noneArm != true) {
			if (coolTime <= 0 && RangeJudge(beamRange) && stopFlag == false && head->GetParent() == boss && attackType == NONE) {
				attackType = BEAM;
			}
			else if (coolTime <= 0 && RangeJudge(pressRange) && stopFlag == false && attackType == NONE) {
				attackType = PRESS;
			}
		}
		else {
			if (coolTime <= 0 && rushCount != 3 && stopFlag == false && attackType == NONE && head->GetParent() == boss) {
				attackType = RUSH;
			}
			else if (coolTime <= 0 && rushCount == 3 && stopFlag == false && attackType == NONE && head->GetParent() == boss) {
				attackType = BEAM;
			}
			else if (coolTime <= 0 && stopFlag == false && attackType == NONE) {
				attackType = RUSH;
			}
		}

		if (attackType == BEAM) {
			BeamAttack();
		}
		else if (attackType == PRESS) {
			PressAttack();
		}
		else if (attackType == RUSH) {
			Rush();
		}

		PosCorrection();
	}

	boss->Update();
	head->Update();
	body->Update();
	rightarm->Update();
	leftarm->Update();
	rightleg->Update();
	leftleg->Update();
	bullet->Update();
	shockWave->Update();
}

void Boss::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	head->Draw();
	body->Draw();
	rightarm->Draw();
	leftarm->Draw();
	rightleg->Draw();
	leftleg->Draw();
	if (attackType == BEAM) {
		bullet->Draw();
	}
	if (shockFlag == true) {
		shockWave->Draw();
	}
	ModelDraw::PostDraw();
}

void Boss::HitDamage(int part, int damage)
{
	hp -= damage;
	parthp[part] -= damage;
}

void Boss::Fall(int part)
{
	Vector3 pos;
	Vector3 fallspeed(0.0f, -0.5f, 0.0f);
	//頭
	if (part == Parts::head)
	{
		pos = head->GetPos();
		if (pos.y > -30)
		{
			head->SetPos(head->GetPos() + fallspeed);
		}
	}
	//からだ
	if (part == Parts::body)
	{
		pos = body->GetPos();
		if (pos.y > -20)
		{
			body->SetPos(body->GetPos() + fallspeed);
		}
	}
	//みぎうで
	if (part == Parts::rightarm)
	{
		pos = rightarm->GetPos();
		if (pos.y > 5)
		{
			rightarm->SetPos(rightarm->GetPos() + fallspeed);
		}
	}
	//ひだりうで
	if (part == Parts::leftarm)
	{
		pos = leftarm->GetPos();
		if (pos.y > -15)
		{
			leftarm->SetPos(leftarm->GetPos() + fallspeed);
		}
	}
	//みぎあし
	if (part == Parts::rightleg)
	{
		pos = rightleg->GetPos();
		if (pos.y > 0)
		{
			rightleg->SetPos(rightleg->GetPos() + fallspeed);
		}
	}
	//ひだりあし
	if (part == Parts::leftleg)
	{
		pos = leftleg->GetPos();
		if (pos.y > 0)
		{
			leftleg->SetPos(leftleg->GetPos() + fallspeed);
		}
	}
}

void Boss::Move() {
	float posX = boss->GetPos().x;
	float posY = boss->GetPos().y;
	float posZ = boss->GetPos().z;
	float playerPosX = player->GetPos().x;
	float playerPosZ = player->GetPos().z;

	float distanceX = 0;
	float distanceZ = 0;

	distanceX = posX - playerPosX;
	distanceZ = posZ - playerPosZ;

	posX -= distanceX / move;
	posZ -= distanceZ / move;

	boss->SetPos(Vector3(posX, posY, posZ));
}

void Boss::Direction() {
	/// <summary>
	/// 徐々に向く処理
	/// </summary>
	//const float direction = 90.0f;
	//const float rotPower = 0.5f;
	//Vector3 pos = boss->GetPos();
	//Vector3 playerPos = player->GetPos();

	//Vector3 directionVector = pos - playerPos;
	//directionVector.Normalize();

	////ボスの正面から少し前を求める
	//XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y - 90.0f)));
	//movement = XMVector3TransformNormal(movement, matRot);a
	//matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y)));

	//movement *= XMVECTOR{ -1, -1, -1 };

	//Vector3 bossFront = pos + movement * XMVECTOR{ 50, 50, 50 };
	//float playerTheta = atan2(pos.z - playerPos.z, pos.x - playerPos.x);
	//playerTheta = (playerTheta * 180.0f / 3.14f);

	//float bossTheta = atan2(pos.z - bossFront.z, pos.x - bossFront.x);
	//bossTheta = (bossTheta * 180.0f / 3.14);

	////angle = -playerTheta + 180.0f;

	//if (angle > 180.0f) {
	//	angle = -180.0f;
	//}
	//else if (angle < -180.0f) {
	//	angle = 180.0f;
	//}

	//if (playerTheta < bossTheta) {
	//	angle += rotPower;
	//}
	//else {
	//	angle -= rotPower;
	//}

	//boss->SetRotation(Vector3(0.0f, angle, 0.0f));

	/// <summary>
	/// 即向く処理
	/// </summary>
	const float direction = 90.0f;
	Vector3 pos = boss->GetPos();
	Vector3 playerPos = player->GetPos();

	Vector3 distance = pos - playerPos;

	angle = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;

	boss->SetRotation(Vector3(0.0f, angle, 0.0f));
}

bool Boss::RangeJudge(float actionRange) {
	float distanceX, distanceZ;
	float bossRange;
	Vector3 playerPos = player->GetPos();
	Vector3 bossPos = boss->GetPos();

	distanceX = bossPos.x - playerPos.x;
	distanceZ = bossPos.z - playerPos.z;

	bossRange = sqrtf((distanceX * distanceX) + (distanceZ * distanceZ));

	return bossRange >= actionRange;
}

void Boss::BeamAttack() {
	//説明変数
	const float shotSpeed = 10.0f;
	const float timeOver = 0.0f;
	const float initCharge = 40.0f;
	const float initAttack = 80.0f;
	const int initRushCount = 0;
	const float initCoolTime = 80.0f;
	const Vector3 initScale = { 0.5f, 0.5f, 0.5f };
	const Vector3 maxScale = { 15.0f, 15.0f, 15.0f };

	//攻撃用メンバ変数
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
		chargeTime = initCharge;
		attackTime = initAttack;
		bulletScale = initScale;
	}
	attackFlag = true;

	//攻撃用ローカル変数
	Vector3 direction = oldBossPos - oldPlayerPos;
	direction.Normalize();

	//ボスの正面から少し前を求める
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y - 90.0f)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y)));

	XMVECTOR bossFront = oldBossPos + movement * XMVECTOR{ 50, 50, 50 };

	//振動
	if (chargeTime >= timeOver) {
		if (chargeTime == initCharge) {
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Charge], Audio::not, 0.2f);
		}
		chargeTime -= 1.0f;
		bulletPos = bossFront;
		if (bulletScale.x < maxScale.x) {
			bulletScale += initScale;
		}
		bullet->SetPos(bulletPos);
		bullet->SetScale(bulletScale);
		shakePosX = oldBossPos.x + rand() % 4 - 2;
		shakePosZ = oldBossPos.z + rand() % 4 - 2;
		boss->SetPos(Vector3(shakePosX, oldBossPos.y, shakePosZ));
	}
	if (chargeTime <= timeOver && attackTime >= timeOver) {
		if (attackTime == initAttack) {
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Shot], Audio::not, 0.5f);
		}
		attackTime -= 1.0f;
		bulletPos -= direction * shotSpeed;
		bullet->SetPos(bulletPos);
	}
	if (attackTime <= timeOver) {
		if (rushCount == 3) {
			rushCount = initRushCount;
		}
		chargeTime = initCharge;
		coolTime = initAttack;
		boss->SetPos(oldBossPos);
		attackTime = initAttack;
		attackType = NONE;
		attackFlag = false;
	}
}

void Boss::PressAttack() {
	//説明変数
	const float timeOver = 0.0f;
	const float initCharge = 20.0f;
	const float initAttack = 20.0f;
	const float initCoolTime = 80.0f;
	const float maxShockScale = 100.0f;
	const float initPressWaitTime = 10.0f;
	const Vector3 initShockScale = { 0.0f, 0.0f, 0.0f };

	//攻撃用メンバ変数
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
		pressPos = boss->GetPos();
		pressPower = 5.0f;
		chargeTime = initCharge;
	}
	attackFlag = true;
	timer++;
	//プレス攻撃
	if (chargeTime >= timeOver) {
		chargeTime -= 1.0f;
		shakePosX = oldBossPos.x + rand() % 4 - 2;
		shakePosZ = oldBossPos.z + rand() % 4 - 2;
		shockPos = boss->GetPos();
		shockPos.y = shockPos.y - 4.0f;
		boss->SetPos(Vector3(shakePosX, oldBossPos.y, shakePosZ));
		shockWave->SetPos(shockPos);
	}
	if (chargeTime <= timeOver) {
		pressPower -= 0.5f;
		pressPos.y += pressPower;
		if (shockFlag == false) {
			boss->SetPos(pressPos);
		}
	}
	if (boss->GetPos().y <= oldBossPos.y && pressPower <= 0 && pressWaitFlag == false) {
		if (shockFlag == false) {
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Shot], Audio::not, 0.5f);
			shockFlag = true;
		}
		shockScale.x += 5.0f;
		shockScale.z += 5.0f;
		shockWave->SetScale(shockScale);
	}
	if (shockScale.x >= maxShockScale && pressWaitFlag == false) {
		shockScale = initShockScale;
		shockWave->SetScale(shockScale);
		pressWaitFlag = true;
	}
	if (pressWaitFlag == true) {
		pressWaitTime -= 0.1f;
	}
	if (pressWaitTime <= timeOver) {
		chargeTime = initCharge;
		coolTime = initCoolTime;
		pressWaitTime = initPressWaitTime;
		pressWaitFlag = false;
		boss->SetPos(oldBossPos);
		attackType = NONE;
		attackFlag = false;
		shockFlag = false;
		timer = 0;
	}
}

int Boss::damage(float weaponATK) {
	float partsDEF = 0.8f;

	int damage = 0;
	damage = weaponATK * partsDEF;

	return damage;
}

void Boss::PosCorrection() {
	//左脚と右脚が取れているとき
	if (leftleg->GetParent() != boss && rightleg->GetParent() != boss && noneArm != true) {

		noneLeg = true;

		if (correctionPos.y > -0.5f) {
			correctionPos.y -= 0.5f;
		}
		if (attackFlag == false && attackType != PRESS) {
			boss->SetPos(Vector3(boss->GetPos().x, correctionPos.y, boss->GetPos().z));
		}
	}
	else {
		correctionPos = boss->GetPos();
	}

	if (noneLeg == true && leftarm->GetParent() != boss && rightarm->GetParent() != boss) {

		noneArm = true;

		if (correctionPos.y > -3.0f) {
			correctionPos.y -= 0.5f;
		}

		if (head->GetParent() == boss) {
			head->SetPos(Vector3(35.0f, 25.0f, 0.0f));
			head->SetRotation(Vector3(head->GetRotation().x, head->GetRotation().y, 75.0f));
		}
		boss->SetRotation(Vector3(boss->GetRotation().x, boss->GetRotation().y, -75.0f));
		boss->SetPos(Vector3(boss->GetPos().x, correctionPos.y, boss->GetPos().z));
	}
}

void Boss::Rush() {
	float posX = boss->GetPos().x;
	float posY = boss->GetPos().y;
	float posZ = boss->GetPos().z;
	float playerPosX = player->GetPos().x;
	float playerPosZ = player->GetPos().z;

	const float rushSpeed = 8.0f;
	const float initCharge = 25.0f;
	const float initAttack = 80.0f;
	const float initCoolTime = 50.0f;
	const float timeOver = 0.0f;

	float distanceX = 0;
	float distanceZ = 0;

	//攻撃用メンバ変数
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
		chargeTime = initCharge;
		attackTime = initAttack;
	}
	attackFlag = true;

	if (chargeTime >= timeOver) {
		chargeTime -= 1.0f;
		shakePosX = oldBossPos.x + rand() % 4 - 2;
		shakePosZ = oldBossPos.z + rand() % 4 - 2;
		boss->SetPos(Vector3(shakePosX, oldBossPos.y, shakePosZ));
	}
	if (chargeTime <= timeOver && attackTime >= timeOver) {
		if (attackTime == initAttack) {
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Shot], Audio::not, 0.5f);
		}
		attackTime -= 1.0f;

		distanceX = posX - oldPlayerPos.x;
		distanceZ = posZ - oldPlayerPos.z;

		posX -= distanceX / rushSpeed;
		posZ -= distanceZ / rushSpeed;

		boss->SetPos(Vector3(posX, posY, posZ));
	}
	if (attackTime <= timeOver) {
		if (rushCount <= 3) {
			rushCount++;
		}
		chargeTime = initCharge;
		coolTime = initAttack;
		attackTime = initAttack;
		attackType = NONE;
		attackFlag = false;
	}
}