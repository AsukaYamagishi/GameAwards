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
	rightaram = ModelDraw::Create();
	rightaram->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Right_arm));
	leftaram = ModelDraw::Create();
	leftaram->SetModel(ModelManager::GetIns()->GetModel(ModelManager::Left_arm));
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
	rightaram->SetParent(boss);
	leftaram->SetParent(boss);
	rightleg->SetParent(boss);
	leftleg->SetParent(boss);

	rightaram->SetPos(Vector3(0, 11, 32));
	leftaram->SetPos(Vector3(13, -18, 7));
	rightleg->SetPos(Vector3(0, 0, 0));
	leftleg->SetPos(Vector3(0, 0, 0));
	bullet->SetPos(Vector3(0, 0, 0));
	bullet->SetScale(Vector3(20, 20, 20));
}

void Boss::Update()
{
	//�����p�ϐ�
	const float moveRange = 20.0f;
	const float attackRange = 60.0f;
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
	if (RangeJudge(moveRange) && hp > 0 && stopFlag == false && attackFlag == false) {
		Move();
	}
	//�v���C���[�̕�����������
	if (hp > 0 && stopFlag == false) {
		Direction();
	}
	//�U��
	if (coolTime <= 0 && RangeJudge(attackRange) && stopFlag == false) {
		Attack();
	}

	boss->Update();
	head->Update();
	body->Update();
	rightaram->Update();
	leftaram->Update();
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
	rightaram->Draw();
	leftaram->Draw();
	rightleg->Draw();
	leftleg->Draw();
	if (attackFlag == true) {
		
	}
	bullet->Draw();
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
		if (pos.y > -40)
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
	if (part == Parts::rightaram)
	{
		pos = rightaram->GetPos();
		if (pos.y > -20)
		{
			rightaram->SetPos(rightaram->GetPos() + fallspeed);
		}
	}
	//�Ђ��肤��
	if (part == Parts::leftaram)
	{
		pos = leftaram->GetPos();
		if (pos.y > -20)
		{
			leftaram->SetPos(leftaram->GetPos() + fallspeed);
		}
	}
	//�݂�����
	if (part == Parts::rightleg)
	{
		pos = rightleg->GetPos();
		if (pos.y > -10)
		{
			rightleg->SetPos(rightleg->GetPos() + fallspeed);
		}
	}
	//�Ђ��肠��
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
	XMFLOAT3 pos = boss->GetPos();
	XMFLOAT3 playerPos = player->GetPos();

	float distanceX = 0;
	float distanceZ = 0;

	//float angle = 0.0f;
	float direction = 90.0f;

	distanceX = pos.x - playerPos.x;
	distanceZ = pos.z - playerPos.z;

	angle = (atan2(distanceX, distanceZ) * 100.0f) / 3.14f * 2.0f + direction;

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

void Boss::Attack() {
	//�����p�ϐ�
	const float shotSpeed = 10.0f;
	const float timeOver = 0.0f;

	//�U���p���[�J���ϐ�
	if (attackFlag == false) {
		oldBossPos = boss->GetPos();
		oldPlayerPos = player->GetPos();
	}
	attackFlag = true;
	//�{�X����v���C���[�ւ̃x�N�g�������߂�
	Vector3 bossToPlayer = oldPlayerPos - oldBossPos;
	bullet->SetPos(bossToPlayer);

	//�U��
	if (chargeTime >= timeOver) {
		chargeTime -= 1.0f;
		shakePosX = oldBossPos.x + rand() % 4 - 2;
		shakePosZ = oldBossPos.z + rand() % 4 - 2;
		boss->SetPos(Vector3(shakePosX, oldBossPos.y, shakePosZ));
	}
	if (chargeTime <= timeOver) {
		chargeTime = 20.0f;
		coolTime = 100.0f;
		boss->SetPos(oldBossPos);
		attackFlag = false;
	}
}