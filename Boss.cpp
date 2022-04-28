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

	//�R���C�_�[�̒ǉ�
	MeshCollider* headcollider = new MeshCollider;
	MeshCollider* bodycollider = new MeshCollider;
	MeshCollider* rightarmcollider = new MeshCollider;
	MeshCollider* leftarmcollider = new MeshCollider;
	MeshCollider* rightlegcollider = new MeshCollider;
	MeshCollider* leftlegcollider = new MeshCollider;
	//�R���C�_�[�̒ǉ�
	float radius = 0.0f;
	float radius2 = 10.0f;
	//���̓����蔻��
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

	//�^�O�t��
	headcollider->tag = CollisionTag::TagHead;
	bodycollider->tag = CollisionTag::TagBody;
	rightarmcollider->tag = CollisionTag::TagRightArm;
	leftarmcollider->tag = CollisionTag::TagLeftArm;
	rightlegcollider->tag = CollisionTag::TagRightLeg;
	leftlegcollider->tag = CollisionTag::TagLeftLeg;
	bullet->collider->tag= CollisionTag::TagBullet;
}

Boss::~Boss()
{


}

void Boss::Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio, ModelDraw* player)
{
	// nullptr�`�F�b�N
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
	rightarm->SetScale(Vector3(5, 5, 5));
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
	//�����ϐ�
	const float moveRange = 20.0f;
	const float beamRange = 60.0f;
	const float pressRange = 0.0f;
	//�f�o�b�O�p��0�L�[�������ƃ{�X�̓������~�܂�
	if (input->PressKeyTrigger(DIK_0)) {
		if (stopFlag == false) {
			stopFlag = true;
		}
		else {
			stopFlag = false;
		}
	}
	//�N�[���^�C�������Z��������
	coolTime -= 1.0f;
	//�v���C���[�̈�苗���܂ňړ�����
	if (hp > 0) {
		if (RangeJudge(moveRange) && stopFlag == false && attackFlag == false) {
			Move();
		}
		//�v���C���[�̕�����������
		if (stopFlag == false) {
			Direction();
		}
		//�U��
		if (coolTime <= 0 && RangeJudge(beamRange) && stopFlag == false && head->GetParent() != nullptr && attackType == NONE) {
			attackType = BEAM;
		}
		else if (coolTime <= 0 && RangeJudge(pressRange) && stopFlag == false && attackType == NONE) {
			attackType = PRESS;
		}

		if (attackType == BEAM) {
			BeamAttack();
		}
		else if (attackType == PRESS) {
			PressAttack();
		}
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
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	ModelDraw::PreDraw(cmdList);
	head->Draw();
	body->Draw();
	rightarm->Draw();
	leftarm->Draw();
	rightleg->Draw();
	leftleg->Draw();
	//if (attackType == BEAM) {
		bullet->Draw();
	//}
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
	//��
	if (part == Parts::head)
	{
		pos = head->GetPos();
		if (pos.y > -30)
		{
			head->SetPos(head->GetPos() + fallspeed);
		}
	}
	//���炾
	if (part == Parts::body)
	{
		pos = body->GetPos();
		if (pos.y > -20)
		{
			body->SetPos(body->GetPos() + fallspeed);
		}
	}
	//�݂�����
	if (part == Parts::rightarm)
	{
		pos = rightarm->GetPos();
		if (pos.y > 5)
		{
			rightarm->SetPos(rightarm->GetPos() + fallspeed);
		}
	}
	//�Ђ��肤��
	if (part == Parts::leftarm)
	{
		pos = leftarm->GetPos();
		if (pos.y > -15)
		{
			leftarm->SetPos(leftarm->GetPos() + fallspeed);
		}
	}
	//�݂�����
	if (part == Parts::rightleg)
	{
		pos = rightleg->GetPos();
		if (pos.y > 0)
		{
			rightleg->SetPos(rightleg->GetPos() + fallspeed);
		}
	}
	//�Ђ��肠��
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
	const float direction = 90.0f;
	const float rotPower = 0.5f;
	Vector3 pos = boss->GetPos();
	Vector3 playerPos = player->GetPos();

	Vector3 directionVector = pos - playerPos;
	directionVector.Normalize();

	//�{�X�̐��ʂ��班���O�����߂�
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y - 90.0f)));
	movement = XMVector3TransformNormal(movement, matRot);
	matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y)));

	movement *= XMVECTOR{ -1, -1, -1 };

	Vector3 bossFront = pos + movement * XMVECTOR{ 50, 50, 50 };
	float playerTheta = atan2(pos.z - playerPos.z, pos.x - playerPos.x);
	playerTheta = (playerTheta * 180.0f / 3.14f);

	float bossTheta = atan2(pos.z - bossFront.z, pos.x - bossFront.x);
	bossTheta = (bossTheta * 180.0f / 3.14);

	//angle = -playerTheta + 180.0f;

	if (angle > 180.0f) {
		angle = -180.0f;
	}
	else if (angle < -180.0f) {
		angle = 180.0f;
	}

	if (playerTheta < bossTheta) {
		angle += rotPower;
	}
	else {
		angle -= rotPower;
	}

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
	//�����ϐ�
	const float shotSpeed = 10.0f;
	const float timeOver = 0.0f;
	const float initCharge = 30.0f;
	const float initAttack = 100.0f;
	const float initCoolTime = 100.0f;

	//�U���p�����o�ϐ�
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
		chargeTime = initCharge;
		attackTime = initAttack;
	}
	attackFlag = true;

	//�U���p���[�J���ϐ�
	Vector3 direction = oldBossPos - oldPlayerPos;
	direction.Normalize();

	//�{�X�̐��ʂ��班���O�����߂�
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y - 90.0f)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	matRot = XMMatrixRotationY((XMConvertToRadians(boss->GetRotation().y)));

	XMVECTOR bossFront = oldBossPos + movement * XMVECTOR{ 50, 50, 50 };

	//�U��
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
		attackTime = initAttack;
		attackType = NONE;
		attackFlag = false;
	}
}

void Boss::PressAttack() {
	//�����ϐ�
	const float timeOver = 0.0f;
	const float initCharge = 20.0f;
	const float initAttack = 20.0f;
	const float initCoolTime = 100.0f;

	//�U���p�����o�ϐ�
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
		pressPos = boss->GetPos();
		pressPower = 5.0f;
		chargeTime = initCharge;
	}
	attackFlag = true;

	//�v���X�U��
	if (chargeTime >= timeOver) {
		chargeTime -= 1.0f;
		shakePosX = oldBossPos.x + rand() % 4 - 2;
		shakePosZ = oldBossPos.z + rand() % 4 - 2;
		boss->SetPos(Vector3(shakePosX, oldBossPos.y, shakePosZ));
	}
	if (chargeTime <= timeOver) {
		pressPower -= 0.5f;
		pressPos.y += pressPower;
		boss->SetPos(pressPos);
	}
	if (boss->GetPos().y <= oldBossPos.y && pressPower <= 0) {
		audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[Shot], Audio::not, 0.5f);
		chargeTime = initCharge;
		coolTime = initCoolTime;
		boss->SetPos(oldBossPos);
		attackType = NONE;
		attackFlag = false;
	}
}