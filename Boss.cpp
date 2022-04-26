#include "Boss.h"

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

	soundSE[Charge] = audio->SoundLoadWave("Resources/Sound/SE/Charge.wav");
	soundSE[Shot] = audio->SoundLoadWave("Resources/Sound/SE/WeaponAttack_Boss01.wav");
}

void Boss::Update()
{
	//説明用変数
	const float moveRange = 20.0f;
	const float beamRange = 60.0f;
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
	if (RangeJudge(moveRange) && hp > 0 && stopFlag == false && attackFlag == false) {
		Move();
	}
	//プレイヤーの方を見続ける
	if (hp > 0 && stopFlag == false) {
		Direction();
	}
	//攻撃
	if (coolTime <= 0 && RangeJudge(beamRange) && stopFlag == false) {
		BeamAttack();
	}

	boss->Update();
	head->Update();
	body->Update();
	rightarm->Update();
	leftarm->Update();
	rightleg->Update();
	leftleg->Update();
	bullet->Update();
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
	if (attackFlag == true) {
		bullet->Draw();
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
		if (pos.y > -40)
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
		if (pos.y > -20)
		{
			rightarm->SetPos(rightarm->GetPos() + fallspeed);
		}
	}
	//ひだりうで
	if (part == Parts::leftarm)
	{
		pos = leftarm->GetPos();
		if (pos.y > -20)
		{
			leftarm->SetPos(leftarm->GetPos() + fallspeed);
		}
	}
	//みぎあし
	if (part == Parts::rightleg)
	{
		pos = rightleg->GetPos();
		if (pos.y > -10)
		{
			rightleg->SetPos(rightleg->GetPos() + fallspeed);
		}
	}
	//ひだりあし
	if (part == Parts::leftleg)
	{
		pos = leftleg->GetPos();
		if (pos.y > -10)
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
	Vector3 pos = boss->GetPos();
	Vector3 playerPos = player->GetPos();

	Vector3 distance = pos - playerPos;

	//float angle = 0.0f;
	float direction = 90.0f;

	angle = (atan2(distance.x, distance.z) * 100.0f) / 3.14f * 2.0f + direction;

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
	//説明用変数
	const float shotSpeed = 5.0f;
	const float timeOver = 0.0f;
	const float initCharge = 30.0f;
	const float initAttack = 100.0f;

	//攻撃用メンバ変数
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
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
		bullet->SetPos(bulletPos);
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
		chargeTime = initCharge;
		coolTime = initAttack;
		boss->SetPos(oldBossPos);
		attackTime = 100.0f;
		attackFlag = false;
	}
}