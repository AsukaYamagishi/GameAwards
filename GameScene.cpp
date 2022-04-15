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
	//safe_delete(testModel);
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
	testModel = FbxInput::GetInstance()->LoadFbxFromFile("Right_arm");
	//3Dオブジェクト生成とモデルのセット
	testObject = new FbxDraw();
	testObject->Init();
	testObject->SetModel(testModel.get());
	testObject->SetScale({ 0.01,0.0001,0.001 });
	testObject->SetRotation({ 0,0,0 });
	testObject->SetPosition({ 0,5,3 });
	//testObject->PlayAnimation(true);

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


	//パーティクルの生成
	if (input->PressKeyTrigger(DIK_SPACE)) {
		//for (int i = 0; i < 50; i++) {
		//	//X,Y,Z全て[-0.5f, +0.5f]でランダムに分布
		//	const float rnd_pos = 10.0f;
		//	XMFLOAT3 pos{};
		//	pos.x = 0;
		//	pos.y = 0;
		//	pos.z = 10;
		//	//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		//	const float rnd_vel = 0.1f;
		//	XMFLOAT3 vel{};
		//	vel.x = (float)(rand() % 300 - 100) / 100.0f;
		//	vel.y = (float)(rand() % 300 - 100) / 100.0f;
		//	vel.z = (float)(rand() % 300 - 100) / 100.0f;
		//	//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		//	XMFLOAT3 acc{};
		//	const float rnd_acc = 0.001f;
		//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		//	//追加
		//	particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		//}
	}

	if (input->PressKey(DIK_Z)) {
		angle += radius;
	}
	else if (input->PressKey(DIK_C)) {
		angle -= radius;
	}

	;

	particleMan->Update();



#pragma region 当たり判定
	Capsule capsule(Vector3(-5, +10, -30), Vector3(+5, -10, -20), 5, (0, 255, 255));

	//体の各部位のカプセル
	Capsule headCapsule(Vector3(0, +50, 0), Vector3(0, 40, 0), 3, (0, 255, 255));
	Capsule bodyCapsule(Vector3(0, 50, 5), Vector3(0, 20, 5), 6, (0, 255, 255));
	Capsule rightAramCapsule(Vector3(-10, 50, 5), Vector3(-20, 5, -5), 5, (0, 255, 255));
	Capsule leftAramCapsule(Vector3(20, 50, 5), Vector3(20, 5, 20), 1, (0, 255, 255));
	Capsule rightLegCapsule(Vector3(-5, 20, 5), Vector3(-5, 0, 5), 1, (0, 255, 255));
	Capsule leftLegCapsule(Vector3(5, 20, 5), Vector3(+5, 0, 5), 1, (0, 255, 255));

	//プレイヤーの攻撃範囲
	Vector3 startpoint = player->player->GetPos();
	Vector3 endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 5.0f);
	Capsule attackCapsule(startpoint, endpoint, 3, (0, 255, 255));
	int damage = 1;
	//敵のパーツ保持中（今回は左腕）
	if (player->enemyWepon == true)
	{
		endpoint = player->player->GetPos() + Vector3(0.0f, 0.0f, 80.0f);
		attackCapsule.endPosition = endpoint;
		attackCapsule.radius = 6;
		damage = 3;
	}

	if (player->attack)
	{
		if (mCollision::testCapsuleCapsule(headCapsule, attackCapsule) && boss->parthp[0] > 0)
		{
			debugText.PrintDebugText("head", 0, 0);
			boss->HitDamage(head, damage);
			player->attack = false;
			particleMan->CreateParticle();
		}
		if (mCollision::testCapsuleCapsule(bodyCapsule, attackCapsule))
		{
			debugText.PrintDebugText("body", 0, 15);
			boss->HitDamage(body, damage);
			player->attack = false;
			particleMan->CreateParticle();
		}
		if (mCollision::testCapsuleCapsule(rightAramCapsule, attackCapsule) && boss->parthp[2] > 0)
		{
			debugText.PrintDebugText("rightAram", 0, 30);
			boss->HitDamage(rightaram, damage);
			player->attack = false;
			particleMan->CreateParticle();
		}
		if (mCollision::testCapsuleCapsule(leftAramCapsule, attackCapsule) && boss->parthp[3] > 0)
		{
			debugText.PrintDebugText("leftAram", 0, 45);
			boss->HitDamage(leftaram, damage);
			player->attack = false;
			particleMan->CreateParticle();
		}
		if (mCollision::testCapsuleCapsule(rightLegCapsule, attackCapsule) && boss->parthp[4] > 0)
		{
			debugText.PrintDebugText("rightLeg", 0, 60);
			boss->HitDamage(rightleg, damage);
			player->attack = false;
			particleMan->CreateParticle();
		}
		if (mCollision::testCapsuleCapsule(leftLegCapsule, attackCapsule) && boss->parthp[5] > 0)
		{
			debugText.PrintDebugText("leftLeg", 0, 75);
			boss->HitDamage(leftleg, damage);
			player->attack = false;
			particleMan->CreateParticle();
		}
	}
#pragma endregion


	//ボスのHPが 0 になったらパーツのHPも全部 0 にして全部落下させる
	if (boss->hp <= 0)
	{
		for (int i = 0; i < 5; i++)
		{
			boss->parthp[i] = 0;
		}
	}

	//パーツ落下処理
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

	//デバッグ用にパーツに直接ダメージ
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

#pragma region 部位の取得
	Capsule RightAramCapsule2(Vector3(-20, 10, 5), Vector3(-20, -30, 5), 10, (0, 255, 255));
	Capsule playerCapsule(player->player->GetPos(), player->player->GetPos() + Vector3(0.0f, -30.0f, 0.0f), 2, (0, 255, 255));
	if (mCollision::testCapsuleCapsule(RightAramCapsule2, playerCapsule))
	{
		debugText.PrintDebugText("syutoku", 0, 90);
		if (boss->parthp[rightaram] <= 0)
		{
			if (input->PressKey(DIK_R))
			{
				//パーツ取得時にプレイヤーの座標とかに合わせる
				boss->rightaram->SetParent(player->player);
				boss->rightaram->SetPos(Vector3(0, -50, +20));
				boss->rightaram->SetRotation(Vector3(90, -15, 180));
				boss->rightaram->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				player->enemyWepon = true;
			}
		}
	}
#pragma endregion

#pragma region カメラの移動
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
	//カメラの設定
	//camera->eye = player->player->GetPos() + meye;
	//camera->eye.y -= 1.0f;
	//camera->eye.z -= 15.0f;
	//camera->target = player->player->GetPos() + mtarget;
	//camera->target.y = 10.0f;

	//testObject->Update();
	camera->eye = player->player->GetPos(); 
	camera->eye.x = 50 * cosf(angle) + player->player->GetPos().x;
	camera->eye.z = 50 * -sinf(angle) + player->player->GetPos().z;
	camera->target = player->player->GetPos();
	///camera->target.y = 10.0f;

	camera->SetCam(camera);
	camera->Update();

	boss->Update();

#pragma region デバッグテキスト設定
	//int型からatr型へ変換
	std::ostringstream oss;
	oss << boss->hp;
	debugText.PrintDebugText(oss.str(), 500, 0);
#pragma endregion

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList *cmdList = dxCommon->GetCommandList();

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
	//boss->Draw();
	stage->Draw();
	skydome->Draw();
	ParticleManager::PreDraw(cmdList);
	particleMan->Draw();
	ParticleManager::PostDraw();
	testObject->Draw(cmdList);
	
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//前景スプライト描画
	//sprite->Draw();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
