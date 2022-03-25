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
	safe_delete(testModel);
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
	testModel = FbxInput::GetInstance()->LoadFbxFromFile("boneTest");
	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	testObject = new FbxDraw();
	testObject->Init();
	testObject->SetModel(testModel);
	testObject->SetScale({ 10,10,10 });
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
	boss->Initialize(dxCommon, input, audio);
	boss->boss->SetPos(Vector3(0,5,0));
	boss->boss->SetRotation(Vector3(0.0f, 90.0f, 0.0f));

	stage = new OBJObject();
	stage->Initialize(dxCommon, input, audio, ModelManager::Stage);
	skydome = new OBJObject();
	skydome->Initialize(dxCommon, input, audio, ModelManager::Skydome);

	weapon = new OBJObject();
	weapon->Initialize(dxCommon, input, audio, ModelManager::Weapon);
	weapon->model->SetScale({1, 1, 1});
	weapon->model->SetRotation({ 0, 45, 0 });
	weapon->model->SetPos({ -3.0f, -2 , 1.7f });
	weapon->model->SetParent(player->player);
	

	gameEndFlag = false;
}

void GameScene::Update()
{
	/*camera->eye.x = 0;
	camera->eye.y = 50;
	camera->eye.z = -100*/;

	if (input->PressKeyTrigger(DIK_SPACE)) {
		for (int i = 0; i < 50; i++) {
			//X,Y,Z�S��[-0.5f, +0.5f]�Ń����_���ɕ��z
			const float rnd_pos = 10.0f;
			XMFLOAT3 pos{};
			pos.x = 0;
			pos.y = 0;
			pos.z = 10;
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
	particleMan->Update();

	camera->Update();

	stage->model->SetScale({ 30,30,30 });


	player->Update();
	stage->Update();
	skydome->Update();
	weapon->Update();
	testObject->Update();
	camera->eye = player->player->GetPos(); 
	camera->eye.y -= 1.0f;
	camera->eye.z -= 10.0f;
	camera->target = player->player->GetPos();
	//camera->target.y = 10.0f;
	camera->SetCam(camera);
	camera->Update();

	boss->Update();

#pragma region �f�o�b�O�e�L�X�g�ݒ�
#pragma endregion
	
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
	//sprite->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	//debugText.DrawAll(cmdList);
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
#pragma endregion
}
