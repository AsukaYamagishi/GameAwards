#include "GameScene.h"
#include "ParticleManager.h"
#include <cassert>
#include <time.h>
#include "FbxInput.h"
#include "FbxDraw.h"
#include "CollisionManager.h"
#include"CollisionTypes.h"

using namespace DirectX;
enum mesh
{
	Ghead = 5,
	Gbody = 4,
	Grightarm = 3,
	Gleftarm = 2,
	Grightleg = 1,
	Gleftleg = 0,
	headToPlayer = 7,
};

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	//safe_delete�͂����ōs��
	safe_delete(particleMan);
	safe_delete(testObject);
	safe_delete(player);
	safe_delete(boss);
	safe_delete(stage);
	safe_delete(weapon);
	safe_delete(camera);
	safe_delete(collisionManager);
	//safe_delete(testModel);
}

void GameScene::Finalize()
{
	collisionManager->Finalize();
}

void GameScene::Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
#pragma region nullptr�`�F�b�N/���
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
#pragma endregion


	//�J�����̂�����
	camera->Initialize({ 0,30,130 });
	camera = Camera::GetCam();
	camera->target = { 0 ,50 ,0 };
#pragma endregion

#pragma region �f�o�b�O�e�L�X�g�ǂݍ���
	// �f�o�b�O�e�L�X�g�p�e�N�X�`���ǂݍ���
	if (!Sprite::LoadTexture(debugTextTexNum, L"Resources/DebugText.png")) {
		assert(0);
		return;
	}
	// �f�o�b�O�e�L�X�g������
	debugText.Init(debugTextTexNum);

#pragma endregion

#pragma region Sprite�����ݒ�
	// �e�N�X�`���ǂݍ���(�P�Ԃɂ���Ƃ悭�킩���G���[�N����)
	if (!Sprite::LoadTexture(2, L"Resources/sprite/redHP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(3, L"Resources/sprite/blackHP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(4, L"Resources/sprite/playerHP.png")) {
		assert(0);
		return;
	}

	if (!Sprite::LoadTexture(5, L"Resources/sprite/control.png")) {
		assert(0);
		return;
	}
	//// �X�v���C�g����
	boss1HP_Red = Sprite::CreateSprite(2, { 310.0f,10.0f });
	boss1HP_Black = Sprite::CreateSprite(3, { 310.0f,10.0f });
	playerHP = Sprite::CreateSprite(4, { 10, 582 });
	control = Sprite::CreateSprite(5, { 20, 10 });
#pragma endregion
	//�f�o�C�X���Z�b�g
	FbxDraw::SetDevice(dxCommon->GetDevice());
	//�J�������Z�b�g
	FbxDraw::SetCamera(camera);
	//�O���t�B�b�N�X�p�C�v���C������
	FbxDraw::CreateGraphicsPipeline();

#pragma region 3D���f��Create�E�����ݒ�
	//���f�����w�肵�ēǂݍ���
	testModel = FbxInput::GetInstance()->LoadFbxFromFile("Player");
	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	testObject = new FbxDraw();
	testObject->Init();
	testObject->SetModel(testModel.get());
	testObject->SetScale({ 0.03,0.03,0.03 });
	testObject->SetPosition({ 0,5,30 });
	testObject->PlayAnimation(true);

	//�p�[�e�B�N���̐���
	particleMan = ParticleManager::Create();
	particleMan->Update();
#pragma endregion


#pragma region ���y���\�[�X�����ݒ�

	soundNo = 0;

	soundData[0] = audio->SoundLoadWave("Resources/Sound/BGM/Title.wav");
	soundData[1] = audio->SoundLoadWave("Resources/Sound/BGM/Boss_01.wav");
	soundSE[0] = audio->SoundLoadWave("Resources/Sound/SE/Attacked_Boss01.wav");
	soundSE[1] = audio->SoundLoadWave("Resources/Sound/SE/Attacked_Boss02.wav");
	soundSE[2] = audio->SoundLoadWave("Resources/Sound/SE/Attacked_Player.wav");
	soundSE[3] = audio->SoundLoadWave("Resources/Sound/SE/Charge.wav");
	soundSE[4] = audio->SoundLoadWave("Resources/Sound/SE/Disassembly.wav");
	soundSE[5] = audio->SoundLoadWave("Resources/Sound/SE/WeaponAttack_Boss01.wav");
	soundSE[6] = audio->SoundLoadWave("Resources/Sound/SE/WeaponAttack_Normal.wav");
	//audio->SoundPlayWave(audio->xAudio2.Get(), soundData[soundNo], Audio::loop, 0.2f);

#pragma endregion

	player = new Player();
	player->Initialize(dxCommon, input, audio);

	boss = new Boss();
	boss->Initialize(dxCommon, input, audio, player->player);
	boss->boss->SetPos(Vector3(0, 5, 0));
	boss->boss->SetRotation(Vector3(0.0f, 90.0f, 0.0f));

	stage = new OBJObject();
	stage->Initialize(dxCommon, input, audio, ModelManager::Stage);
	stage->model->SetScale({ 30,30,30 });
	skydome = new OBJObject();
	skydome->Initialize(dxCommon, input, audio, ModelManager::Skydome);

	weapon = new Weapon();
	weapon->Initialize(dxCommon, input, audio);
	//�v���C���[�ɒǏ]
	weapon->weapon->SetParent(player->player);


	gameEndFlag = false;

	//�R���W�����}�l�[�W���[�̐���
	collisionManager = CollisionManager::GetInstance();
}

void GameScene::Update()
{


	if (input->PressKey(DIK_RIGHT)) {
		camera->matrot *= XMMatrixRotationY(0.1f);
	}
	else if (input->PressKey(DIK_LEFT)) {
		camera->matrot *= XMMatrixRotationY(-0.1f);
	}

	particleMan->Update();



#pragma region �����蔻��
	//Capsule capsule(Vector3(-5, +10, -30), Vector3(+5, -10, -20), 5, (0, 255, 255));
	////�̂̊e���ʂ̃J�v�Z��
	//Capsule headCapsule(Vector3(0, +50, 0), Vector3(0, 40, 0), 3, (0, 255, 255));
	//Capsule bodyCapsule(Vector3(0, 50, 5), Vector3(0, 20, 5), 6, (0, 255, 255));
	//Capsule rightAramCapsule(Vector3(-10, 50, 5), Vector3(-20, 5, -5), 5, (0, 255, 255));
	//Capsule leftAramCapsule(Vector3(20, 50, 5), Vector3(20, 5, 20), 1, (0, 255, 255));
	//Capsule rightLegCapsule(Vector3(-5, 20, 5), Vector3(-5, 0, 5), 1, (0, 255, 255));
	//Capsule leftLegCapsule(Vector3(5, 20, 5), Vector3(+5, 0, 5), 1, (0, 255, 255));

	////�v���C���[�̍U���͈�
	//Vector3 startpoint = player->player->GetPos();
	//Vector3 endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 5.0f);
	//Capsule attackCapsule(startpoint, endpoint, 3, (0, 255, 255));
	int damage = 1;
	//�G�̃p�[�c�ێ����i����͍��r�j
	if (player->enemyWepon == true)
	{
		/*endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 80.0f);
		attackCapsule.endPosition = endpoint;
		attackCapsule.radius = 6;*/
		damage = 3;
	}
	else
	{
		damage = 1;
	}
#pragma endregion
	if (hit[headToPlayer])
	{
		debugText.PrintDebugText("headToPlayer", 500, 0);
	}

#pragma region �v���C���[�̃_���[�W����
	if (hit[BulletToPlayer]){
		//player->HitDamage();
		debugText.PrintDebugText("aaaaaaaaaaaaaaaaitaiiiiiiiiiiiiiiiii", 0, 0);
	}
#pragma endregion


#pragma region �U������
	if (player->attack)
	{
		if (hit[WwaponToHead] && boss->parthp[head] > 0) {
			boss->HitDamage(head, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToBody]) {
			boss->HitDamage(body, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToRightArm] && boss->parthp[rightarm] > 0) {
			boss->HitDamage(rightarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToLeftArm] && boss->parthp[leftarm] > 0) {
			boss->HitDamage(leftarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToRightLeg] && boss->parthp[rightleg] > 0) {
			boss->HitDamage(rightleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (hit[WwaponToLeftLeg] && boss->parthp[leftleg] > 0) {
			boss->HitDamage(leftleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}

		/*if (mCollision::testCapsuleCapsule(headCapsule, attackCapsule) && boss->parthp[0] > 0)
		{
			debugText.PrintDebugText("head", 0, 0);
			boss->HitDamage(head, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(bodyCapsule, attackCapsule))
		{
			debugText.PrintDebugText("body", 0, 15);
			boss->HitDamage(body, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(rightAramCapsule, attackCapsule) && boss->parthp[2] > 0)
		{
			debugText.PrintDebugText("rightAram", 0, 30);
			boss->HitDamage(rightarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(leftAramCapsule, attackCapsule) && boss->parthp[3] > 0)
		{
			debugText.PrintDebugText("leftAram", 0, 45);
			boss->HitDamage(leftarm, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(rightLegCapsule, attackCapsule) && boss->parthp[4] > 0)
		{
			debugText.PrintDebugText("rightLeg", 0, 60);
			boss->HitDamage(rightleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (mCollision::testCapsuleCapsule(leftLegCapsule, attackCapsule) && boss->parthp[5] > 0)
		{
			debugText.PrintDebugText("leftLeg", 0, 75);
			boss->HitDamage(leftleg, damage);
			player->attack = false;
			particleMan->HitParticle();
		}
		if (!player->oldattack){
			audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[soundNo], Audio::not);
		}*/


		//���b�V���Ƃ�
#pragma region ���b�V���Ƃ�
			//��
		if (hit[Ghead] && boss->parthp[head] > 0)
		{
			debugText.PrintDebugText("head", 0, 0);
			//boss->HitDamage(head, damage);
			player->attack = false;
			//particleMan->HitParticle();			
		}
		//��
		if (hit[Gbody] && boss->parthp[body] > 0)
		{
			debugText.PrintDebugText("body", 0, 15);
			//boss->HitDamage(body, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//�E�r
		if (hit[Grightarm] && boss->parthp[rightarm] > 0)
		{
			debugText.PrintDebugText("rightAram", 0, 30);
			//boss->HitDamage(rightarm, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//���r
		if (hit[Gleftarm] && boss->parthp[leftarm] > 0)
		{
			debugText.PrintDebugText("leftAram", 0, 45);
			//boss->HitDamage(leftarm, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//�E��
		if (hit[Grightleg] && boss->parthp[rightleg] > 0)
		{
			debugText.PrintDebugText("rightLeg", 0, 60);
			//boss->HitDamage(rightleg, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
		//����
		if (hit[Gleftleg] && boss->parthp[leftleg] > 0)
		{
			debugText.PrintDebugText("leftLeg", 0, 75);
			//boss->HitDamage(leftleg, damage);
			player->attack = false;
			//particleMan->HitParticle();
		}
#pragma endregion


		if (!player->oldattack) {
			//audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[soundNo], Audio::not);
		}

	}
#pragma endregion



#pragma region ��������
	//�{�X��HP�� 0 �ɂȂ�����p�[�c��HP���S�� 0 �ɂ��đS������������
	if (boss->hp <= 0)
	{
		for (int i = 0; i < 5; i++)
		{
			boss->parthp[i] = 0;
		}
	}

	//�p�[�c��������
	if (boss->parthp[head] <= 0)
	{
		if (boss->head->GetParent() == boss->boss) {
			boss->head->SetParent(nullptr);
			boss->head->SetPos(boss->boss->GetPos());
		}
		boss->Fall(head);
	}
	if (boss->parthp[body] <= 0)
	{
		if (boss->hp <= 0)
		{
			boss->Fall(body);
		}
	}
	if (boss->parthp[rightarm] <= 0)
	{
		boss->Fall(rightarm);
		if (boss->rightarm->GetParent() == boss->boss) {
			boss->rightarm->SetParent(nullptr);
		}
	}
	if (boss->parthp[leftarm] <= 0)
	{
		if (boss->leftarm->GetParent() == boss->boss) {
			boss->leftarm->SetParent(nullptr);
			boss->leftarm->SetPos(boss->boss->GetPos());
		}
		boss->Fall(leftarm);
	}
	if (boss->parthp[rightleg] <= 0)
	{
		boss->Fall(rightleg);
		if (boss->rightleg->GetParent() == boss->boss) {
			boss->rightleg->SetParent(nullptr);
			boss->rightleg->SetPos(boss->boss->GetPos());
		}

	}
	if (boss->parthp[leftleg] <= 0)
	{
		boss->Fall(leftleg);
		if (boss->leftleg->GetParent() == boss->boss) {
			boss->leftleg->SetParent(nullptr);
			boss->leftleg->SetPos(boss->boss->GetPos());
		}

	}

	//�f�o�b�O�p�Ƀp�[�c�ɒ��ڃ_���[�W
	if (input->PressKeyTrigger(DIK_1))
	{
		boss->parthp[rightarm]--;
	}
	if (input->PressKeyTrigger(DIK_2))
	{
		boss->parthp[leftarm]--;
		particleMan->HitParticle();
	}
	if (input->PressKeyTrigger(DIK_3))
	{
		boss->parthp[rightleg]--;
	}
	if (input->PressKeyTrigger(DIK_4))
	{
		boss->parthp[leftleg]--;
	}
#pragma endregion

#pragma region HP�o�[�̃T�C�Y
	/*-------------�{�X-------------*/
	//float hpSize = (boss1HP_SizeX / boss->hp) * boss->hp;
	float hpSize = (boss1HP_SizeX / boss->maxhp) * boss->hp;
	if (boss->hp < 0) { boss->hp = 0; }
	boss1HP_Red->SetSize({ hpSize, boss1HP_SizeY });

	//char str[256];
	//sprintf_s(str, "hpSize : %f", hpSize);
	//debugText.PrintDebugText(str, 0, 0, 1);

	/*-------------�v���C���[-------------*/
	playerHP->SetSize({ playerHPX, playerHPY });
	playerHP->SetTextureRect({ playerHPX * (playerMaxHp - player->hp),0 }, { 128, 128 });

#pragma endregion

#pragma region ���ʂ̎擾
	if (input->PressKey(DIK_R)) {

		if (hit[WwaponToBody]) {
			//�{�f�B����ꂽ��{�X���S
		}
		if (hit[WwaponToHead] && boss->parthp[head] <= 0 && boss->head->GetParent() == nullptr) {
			boss->head->SetPos(Vector3(0, 0, 0));
			boss->head->SetRotation(Vector3(0, 0, 0));
			boss->head->SetPos(player->player->GetPos());
			boss->head->SetParent(player->player);
			player->enemyWepon = true;
			//�����E�����t���O
			player->headFlag = true;
		}
		if (hit[WwaponToRightArm] && boss->parthp[rightarm] <= 0 && boss->rightarm->GetParent() == nullptr) {
			boss->rightarm->SetPos(Vector3(0, 0, 0));
			boss->rightarm->SetRotation(Vector3(0, 0, 0));
			boss->rightarm->SetParent(player->player);
			player->enemyWepon = true;
		}
		if (hit[WwaponToLeftArm] && boss->parthp[leftarm] <= 0 && boss->leftarm->GetParent() == nullptr) {
			boss->leftarm->SetPos(Vector3(0, 0, 0));
			boss->leftarm->SetRotation(Vector3(0, 0, 0));
			boss->leftarm->SetParent(player->player);
			player->enemyWepon = true;
		}
		if (hit[WwaponToRightLeg] && boss->parthp[rightleg] <= 0 && boss->rightleg->GetParent() == nullptr) {
			boss->rightleg->SetPos(Vector3(0, 0, 0));
			boss->rightleg->SetRotation(Vector3(0, 0, 0));
			boss->rightleg->SetParent(player->player);
			player->enemyWepon = true;
		}
		if (hit[WwaponToLeftLeg] && boss->parthp[leftleg] <= 0 && boss->leftleg->GetParent() == nullptr) {
			boss->leftleg->SetPos(Vector3(0, 0, 0));
			boss->leftleg->SetRotation(Vector3(0, 0, 0));
			boss->leftleg->SetParent(player->player);
			player->enemyWepon = true;
		}
	}


#pragma endregion
#pragma region ����ɂ������ʂ𗎂Ƃ�
	if (input->PressKey(DIK_G))
	{
		//boss->head->SetParent(nullptr);
		//boss->body->SetParent(nullptr);
		//boss->rightarm->SetParent(nullptr);
		//boss->leftarm->SetParent(nullptr);
		//boss->rightleg->SetParent(nullptr);
		//boss->leftleg->SetParent(nullptr);
		player->enemyWepon = false;
		player->headFlag = false;

		//�{�X�̓��͍̂Ō�܂Ŏc��
		if (boss->parthp[body] <= 0)
		{
			if (boss->hp <= 0)
			{
				boss->Fall(body);
			}
		}
		//�p�[�c��������
		if (boss->parthp[head] <= 0)
		{
			if (boss->head->GetParent() == player->player) {
				boss->head->SetParent(nullptr);
				boss->head->SetPos(player->player->GetPos());
			}
			boss->Fall(head);
		}
		if (boss->parthp[rightarm] <= 0)
		{
			boss->Fall(rightarm);
			if (boss->rightarm->GetParent() == player->player) {
				boss->rightarm->SetParent(nullptr);
				boss->rightarm->SetPos(player->player->GetPos());
			}
		}
		if (boss->parthp[leftarm] <= 0)
		{
			if (boss->leftarm->GetParent() == player->player) {
				boss->leftarm->SetParent(nullptr);
				boss->leftarm->SetPos(player->player->GetPos());
			}
			boss->Fall(leftarm);
		}
		if (boss->parthp[rightleg] <= 0)
		{
			boss->Fall(rightleg);
			if (boss->rightleg->GetParent() == player->player) {
				boss->rightleg->SetParent(nullptr);
				boss->rightleg->SetPos(player->player->GetPos());
			}

		}
		if (boss->parthp[leftleg] <= 0)
		{
			boss->Fall(leftleg);
			if (boss->leftleg->GetParent() == player->player) {
				boss->leftleg->SetParent(nullptr);
				boss->leftleg->SetPos(player->player->GetPos());
			}

		}
	}
#pragma endregion


	if (input->PressKeyTrigger(DIK_P)) {
		audio->SoundStop(audio->xAudio2.Get(), Audio::IsLoop::loop);
		if (soundNo < 1) {
			soundNo++;
		}
		else {
			soundNo = 0;
		}
		audio->SoundPlayWave(audio->xAudio2.Get(), soundData[soundNo], Audio::loop, 0.2f);
	}

	if (input->PressKeyTrigger(DIK_L)) {
		audio->SoundPlayWave(audio->xAudio2.Get(), soundSE[seNo]);
		if (seNo < 6) {
			seNo++;
		}
		else {
			seNo = 0;
		}
	}

	debugText.PrintDebugText("WASD:MOVE", 25, 15, 1.5f);
	debugText.PrintDebugText("SHIFT:JUMP", 25, 45, 1.5f);
	debugText.PrintDebugText("SPACE:ATTACK", 25, 75, 1.5f);
	debugText.PrintDebugText("R:PICK UP", 25, 105, 1.5f);
	debugText.PrintDebugText("G:DROP", 25, 135, 1.5f);
	player->Update(*camera);
	stage->Update();
	skydome->Update();
	weapon->Update();
	testObject->Update();


	XMFLOAT3 rote = player->GetNoAttackRotation();
	XMFLOAT3 pos = player->player->GetPos();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matrot);

	movement *= XMVECTOR{ -1, -1, -1 };
	if (player->attack == false)
	{
		matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	}

	camera->eye = player->player->GetPos() + movement * XMVECTOR{ 100, 100, 100 };
	camera->eye.y = 20;
	camera->target = player->player->GetPos();
	//�v���C���[���W�����v���������_������Ɍ����̂�h�~���邽�߂̏���
	camera->target.y = player->graundheight;


	player->Update(*camera);
	stage->Update();
	skydome->Update();
	weapon->Update();
	testObject->Update();
	camera->SetCam(camera);
	camera->Update();
	boss->Update();



#pragma region �f�o�b�O�e�L�X�g�ݒ�
	//int�^����atr�^�֕ϊ�
	std::ostringstream oss;
	std::ostringstream ass;
	std::ostringstream iss;
	std::ostringstream uss;
	oss << boss->hp;
	debugText.PrintDebugText(oss.str(), 500, 0);
	ass << boss->angle;
	debugText.PrintDebugText(ass.str(), 700, 0);
	iss << boss->watch;
	debugText.PrintDebugText(iss.str(), 900, 0);
	uss << boss->watchB;
	debugText.PrintDebugText(uss.str(), 900, 100);


#pragma endregion
	//�t���O�𖈃t���[�����Z�b�g����
	for (int i = 0; i < 20; i++)
	{
		hit[i] = 0;
	}
	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollision(hit);

	//�{�X�����񂾂�G���h�V�[���Ɉڍs
	if (input->PressKeyTrigger(DIK_END) || boss->hp <= 0)
	{
		gameEndFlag = true;
	}
}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region �w�i�X�v���C�g�`��
	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �w�i�X�v���C�g�`��
	//spriteBG->Draw();

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	//�[�x�o�b�t�@�N���A
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D�I�u�W�F�N�g�`��
	// 3D�I�u�W�F�N�g�`��O����
	//Object3D::PreDraw(cmdList);


	// 3D�I�u�N�W�F�N�g�̕`��
	//object3d->Draw();


	// 3D�I�u�W�F�N�g�`��㏈��
	//Object3D::PostDraw();

#pragma endregion

#pragma region 3D���f���`��

	player->Draw();
	weapon->Draw();
	boss->Draw();
	stage->Draw();
	skydome->Draw();
	ParticleManager::PreDraw(cmdList);
	particleMan->Draw();
	ParticleManager::PostDraw();
	//testObject->Draw(cmdList);


#pragma endregion

#pragma region �O�i�X�v���C�g�`��
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	//�O�i�X�v���C�g�`��
	boss1HP_Black->Draw();
	boss1HP_Red->Draw();
	playerHP->Draw();
	control->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
