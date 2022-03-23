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
	//safe_deleteはここで行う
	safe_delete(particleMan);
	safe_delete(testObject);
	safe_delete(testModel);
}

void GameScene::Init(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio)
{
#pragma region nullptrチェック/代入
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
#pragma endregion


	//カメラのせっち
	camera->Initialize({ 0,30,130 });
	camera = Camera::GetCam();
	camera->target = { 0 ,50 ,0 };
#pragma endregion

#pragma region デバッグテキスト読み込み
	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNum, L"Resources/DebugText.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText.Init(debugTextTexNum);

#pragma endregion

#pragma region Sprite初期設定
	// テクスチャ読み込み(１番にするとよくわからんエラー起こる)
	/*if (!Sprite::LoadTexture(3, L"Resources/setumei.png")) {
		assert(0);
		return;
	}*/
	//// 背景スプライト生成
	//sprite = Sprite::CreateSprite(3, { 0.0f,0.0f });
#pragma endregion
	//デバイスをセット
	FbxDraw::SetDevice(dxCommon->GetDevice());
	//カメラをセット
	FbxDraw::SetCamera(camera);
	//グラフィックスパイプライン生成
	FbxDraw::CreateGraphicsPipeline();

#pragma region 3DモデルCreate・初期設定
	//モデルを指定して読み込み
	testModel = FbxInput::GetInstance()->LoadFbxFromFile("boneTest");
	//3Dオブジェクト生成とモデルのセット
	testObject = new FbxDraw();
	testObject->Init();
	testObject->SetModel(testModel);
	testObject->SetScale({ 10,10,10 });
	testObject->PlayAnimation(true);

	//パーティクルの生成
	particleMan = ParticleManager::Create();
	particleMan->Update();
#pragma endregion


#pragma region 音楽リソース初期設定

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
			//X,Y,Z全て[-0.5f, +0.5f]でランダムに分布
			const float rnd_pos = 10.0f;
			XMFLOAT3 pos{};
			pos.x = 0;
			pos.y = 0;
			pos.z = 10;
			//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
			const float rnd_vel = 0.1f;
			XMFLOAT3 vel{};
			vel.x = (float)(rand() % 300 - 100) / 100.0f;
			vel.y = (float)(rand() % 300 - 100) / 100.0f;
			vel.z = (float)(rand() % 300 - 100) / 100.0f;
			//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
			XMFLOAT3 acc{};
			const float rnd_acc = 0.001f;
			acc.y = -(float)rand() / RAND_MAX * rnd_acc;

			//追加
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

#pragma region デバッグテキスト設定
#pragma endregion
	
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 背景スプライト描画
	//spriteBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

	//深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	//Object3D::PreDraw(cmdList);
	

	// 3Dオブクジェクトの描画
	//object3d->Draw();
	

	// 3Dオブジェクト描画後処理
	//Object3D::PostDraw();
	
#pragma endregion

#pragma region 3Dモデル描画
	
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

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//前景スプライト描画
	//sprite->Draw();
	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
