#include "GameScene.h"
#include "ParticleManager.h"
#include <cassert>
#include <time.h>
#include "FbxInput.h"
#include "FbxDraw.h"

using namespace DirectX;


GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	//safe_delete�͂����ōs��
	safe_delete(particleMan);
	safe_delete(testObject);
	//safe_delete(testModel);
}

void GameScene::Init(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio)
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
	/*if (!Sprite::LoadTexture(3, L"Resources/setumei.png")) {
		assert(0);
		return;
	}*/
	//// �w�i�X�v���C�g����
	//sprite = Sprite::CreateSprite(3, { 0.0f,0.0f });
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

	soundData[0] = audio->SoundLoadWave("Resources/musicloop.wav");
	//audio->SoundPlayWave(audio->xAudio2.Get(), soundData[0], Audio::loop);

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

	weapon = new OBJObject();
	weapon->Initialize(dxCommon, input, audio, ModelManager::Weapon);
	weapon->model->SetScale({ 1, 1, 1 });
	weapon->model->SetRotation({ 0, 45, 0 });
	weapon->model->SetPos({ -3.0f, -2 , 1.7f });
	weapon->model->SetParent(player->player);


	gameEndFlag = false;
}

void GameScene::Update()
{


	//�p�[�e�B�N���̐���
	if (input->PressKeyTrigger(DIK_SPACE)) {
		//for (int i = 0; i < 50; i++) {
		//	//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
		//	const float rnd_pos = 10.0f;
		//	XMFLOAT3 pos{};
		//	pos.x = 0;
		//	pos.y = 0;
		//	pos.z = 10;
		//	//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		//	const float rnd_vel = 0.1f;
		//	XMFLOAT3 vel{};
		//	vel.x = (float)(rand() % 300 - 100) / 100.0f;
		//	vel.y = (float)(rand() % 300 - 100) / 100.0f;
		//	vel.z = (float)(rand() % 300 - 100) / 100.0f;
		//	//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		//	XMFLOAT3 acc{};
		//	const float rnd_acc = 0.001f;
		//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//	//�ǉ�
		//	particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		//}
	}

	if (input->PressKey(DIK_Z)) {
		angle += radius;
	}
	else if (input->PressKey(DIK_C)) {
		angle -= radius;
	}


	

	particleMan->Update();



#pragma region �����蔻��
	Capsule capsule(Vector3(-5, +10, -30), Vector3(+5, -10, -20), 5, (0, 255, 255));

	//�̂̊e���ʂ̃J�v�Z��
	Capsule headCapsule(Vector3(0, +50, 0), Vector3(0, 40, 0), 3, (0, 255, 255));
	Capsule bodyCapsule(Vector3(0, 50, 5), Vector3(0, 20, 5), 6, (0, 255, 255));
	Capsule rightAramCapsule(Vector3(-10, 50, 5), Vector3(-20, 5, -5), 5, (0, 255, 255));
	Capsule leftAramCapsule(Vector3(20, 50, 5), Vector3(20, 5, 20), 1, (0, 255, 255));
	Capsule rightLegCapsule(Vector3(-5, 20, 5), Vector3(-5, 0, 5), 1, (0, 255, 255));
	Capsule leftLegCapsule(Vector3(5, 20, 5), Vector3(+5, 0, 5), 1, (0, 255, 255));

	//�v���C���[�̍U���͈�
	Vector3 startpoint = player->player->GetPos();
	Vector3 endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 5.0f);
	Capsule attackCapsule(startpoint, endpoint, 3, (0, 255, 255));
	int damage = 1;
	//�G�̃p�[�c�ێ����i����͍��r�j
	if (player->enemyWepon == true)
	{
		endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 80.0f);
		attackCapsule.endPosition = endpoint;
		attackCapsule.radius = 6;
		damage = 3;
	}


	//debugText.PrintDebugText("rightAram", 0, 0);
	//debugText.PrintDebugText("rightAram", 0, 15);
	//debugText.PrintDebugText("rightAram", 0, 10);
	if (player->attack)
	{
		if (mCollision::testCapsuleCapsule(headCapsule, attackCapsule) && boss->parthp[0] > 0)
		{
			debugText.PrintDebugText("head", 0, 0);			
			boss->HitDamage(head, damage);
			player->attack = false;

			for (int i = 0; i < 50; i++) {
				//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				vel.x = (float)(rand() % 300 - 100) / 100.0f;
				vel.y = (float)(rand() % 300 - 100) / 100.0f;
				vel.z = (float)(rand() % 300 - 100) / 100.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
			}
		}
		if (mCollision::testCapsuleCapsule(bodyCapsule, attackCapsule))
		{
			debugText.PrintDebugText("body", 0, 15);			
			boss->HitDamage(body, damage);
			player->attack = false;

			for (int i = 0; i < 50; i++) {
				//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				vel.x = (float)(rand() % 300 - 100) / 100.0f;
				vel.y = (float)(rand() % 300 - 100) / 100.0f;
				vel.z = (float)(rand() % 300 - 100) / 100.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
			}
		}
		if (mCollision::testCapsuleCapsule(rightAramCapsule, attackCapsule) && boss->parthp[2] > 0)
		{
			debugText.PrintDebugText("rightAram", 0, 30);			
			boss->HitDamage(rightaram, damage);
			player->attack = false;

			for (int i = 0; i < 50; i++) {
				//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				vel.x = (float)(rand() % 300 - 100) / 100.0f;
				vel.y = (float)(rand() % 300 - 100) / 100.0f;
				vel.z = (float)(rand() % 300 - 100) / 100.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
			}
		}


		if (mCollision::testCapsuleCapsule(leftAramCapsule, attackCapsule) && boss->parthp[3] > 0)
		{
			debugText.PrintDebugText("leftAram", 0, 45);			
			boss->HitDamage(leftaram, damage);
			player->attack = false;

			for (int i = 0; i < 50; i++) {
				//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				vel.x = (float)(rand() % 300 - 100) / 100.0f;
				vel.y = (float)(rand() % 300 - 100) / 100.0f;
				vel.z = (float)(rand() % 300 - 100) / 100.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
			}
		}


		if (mCollision::testCapsuleCapsule(rightLegCapsule, attackCapsule) && boss->parthp[4] > 0)
		{
			debugText.PrintDebugText("rightLeg", 0, 60);			
			boss->HitDamage(rightleg, damage);
			player->attack = false;

			for (int i = 0; i < 50; i++) {
				//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				vel.x = (float)(rand() % 300 - 100) / 100.0f;
				vel.y = (float)(rand() % 300 - 100) / 100.0f;
				vel.z = (float)(rand() % 300 - 100) / 100.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
			}
		}
		if (mCollision::testCapsuleCapsule(leftLegCapsule, attackCapsule) && boss->parthp[5] > 0)
		{
			debugText.PrintDebugText("leftLeg", 0, 75);
			boss->HitDamage(leftleg, damage);
			player->attack = false;

			for (int i = 0; i < 50; i++) {
				//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
				const float rnd_pos = 10.0f;
				XMFLOAT3 pos{};
				pos.x = 0;
				pos.y = 0;
				pos.z = 0;
				//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
				const float rnd_vel = 0.1f;
				XMFLOAT3 vel{};
				vel.x = (float)(rand() % 300 - 100) / 100.0f;
				vel.y = (float)(rand() % 300 - 100) / 100.0f;
				vel.z = (float)(rand() % 300 - 100) / 100.0f;
				//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
				XMFLOAT3 acc{};
				const float rnd_acc = 0.001f;
				acc.y = -(float)rand() / RAND_MAX * rnd_acc;

				//�ǉ�
				particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
			}
		}
	}

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
		boss->Fall(head);
	}
	if (boss->parthp[body] <= 0)
	{
		if (boss->hp <= 0)
		{
			boss->Fall(body);
		}
	}
	if (boss->parthp[rightaram] <= 0)
	{
		boss->Fall(rightaram);
	}
	if (boss->parthp[leftaram] <= 0)
	{
		boss->Fall(leftaram);
	}
	if (boss->parthp[rightleg] <= 0)
	{
		boss->Fall(rightleg);
	}
	if (boss->parthp[leftleg] <= 0)
	{
		boss->Fall(leftleg);
	}
#pragma endregion

	//�f�o�b�O�p�Ƀp�[�c�ɒ��ڃ_���[�W
	if (input->PressKeyTrigger(DIK_1))
	{
		boss->parthp[rightaram]--;
	}
	if (input->PressKeyTrigger(DIK_2))
	{
		boss->parthp[leftaram]--;
		particleMan->CreateParticle();
	}
	if (input->PressKeyTrigger(DIK_3))
	{
		boss->parthp[rightleg]--;
	}
	if (input->PressKeyTrigger(DIK_4))
	{
		boss->parthp[leftleg]--;
	}

#pragma region ���ʂ̎擾
	Capsule RightAramCapsule2(Vector3(-20, 10, 5), Vector3(-20, -30, 5), 10, (0, 255, 255));
	Capsule playerCapsule(player->player->GetPos(), player->player->GetPos() + Vector3(0.0f, -30.0f, 0.0f), 2, (0, 255, 255));
	if (mCollision::testCapsuleCapsule(RightAramCapsule2, playerCapsule))
	{
		debugText.PrintDebugText("syutoku", 0, 90);
		if (boss->parthp[rightaram] <= 0)
		{
			if (input->PressKey(DIK_R))
			{
				//�p�[�c�擾���Ƀv���C���[�̍��W�Ƃ��ɍ��킹��
				boss->rightaram->SetParent(player->player);
				boss->rightaram->SetPos(Vector3(-10, -20, +20));
				boss->rightaram->SetRotation(Vector3(90, -15, 180));
				boss->rightaram->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				player->enemyWepon = true;
			}
		}
	}
#pragma endregion

#pragma region �J�����̈ړ�
	//if (input->PressKey(DIK_UP))
	//{
	//	meye.z += 1;
	//	mtarget.z += 1;
	//}

	//if (input->PressKey(DIK_DOWN))
	//{
	//	meye.z -= 1;
	//	mtarget.z -= 1;
	//}
	//if (input->PressKey(DIK_RIGHT))
	//{
	//	meye.x += 1;
	//	mtarget.x += 1;
	//}
	//if (input->PressKey(DIK_LEFT))
	//{
	//	meye.x -= 1;
	//	mtarget.x -= 1;
	//}
#pragma endregion

	player->Update();
	stage->Update();
	skydome->Update();
	weapon->Update();

	testObject->Update();
	//�J�����̐ݒ�
	//camera->eye = player->player->GetPos() + meye;
	//camera->eye.y -= 1.0f;
	//camera->eye.z -= 15.0f;
	//camera->target = player->player->GetPos() + mtarget;
	//camera->target.y = 10.0f;

	//testObject->Update();
	XMFLOAT3 rote = player->player->GetRotation();
	XMFLOAT3 pos = player->player->GetPos();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	camera->eye = player->player->GetPos() + movement * XMVECTOR{ 100, 100, 100 };
	camera->eye.y -= 5;
	camera->target = player->player->GetPos();
	///camera->target.y = 10.0f;

	camera->SetCam(camera);
	camera->Update();

	boss->Update();

#pragma region �f�o�b�O�e�L�X�g�ݒ�
	//int�^����atr�^�֕ϊ�
	std::ostringstream oss;
	oss << boss->hp;
	debugText.PrintDebugText(oss.str(), 500, 0);
#pragma endregion

}

void GameScene::Draw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

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
	//sprite->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	debugText.DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
