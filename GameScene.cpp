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
	//// スプライト生成
	boss1HP_Red = Sprite::CreateSprite(2, { 210.0f,10.0f });
	boss1HP_Black = Sprite::CreateSprite(3, { 210.0f,10.0f });
	playerHP = Sprite::CreateSprite(4, { 10, 582 });
#pragma endregion
	//デバイスをセット
	FbxDraw::SetDevice(dxCommon->GetDevice());
	//カメラをセット
	FbxDraw::SetCamera(camera);
	//グラフィックスパイプライン生成
	FbxDraw::CreateGraphicsPipeline();

#pragma region 3DモデルCreate・初期設定
	//モデルを指定して読み込み
	testModel = FbxInput::GetInstance()->LoadFbxFromFile("Player");
	//3Dオブジェクト生成とモデルのセット
	testObject = new FbxDraw();
	testObject->Init();
	testObject->SetModel(testModel.get());
	testObject->SetScale({ 0.03,0.03,0.03 });
	testObject->SetPosition({ 0,5,30 });
	testObject->PlayAnimation(true);

	//パーティクルの生成
	particleMan = ParticleManager::Create();
	particleMan->Update();
#pragma endregion


#pragma region 音楽リソース初期設定

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
	audio->SoundPlayWave(audio->xAudio2.Get(), soundData[soundNo], Audio::loop, 0.2f);

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


	if (input->PressKey(DIK_Z)) {
		angle += radius;
	}
	else if (input->PressKey(DIK_C)) {
		angle -= radius;
	}

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
		boss->head->SetParent(nullptr);
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
		boss->rightarm->SetParent(nullptr);
	}
	if (boss->parthp[leftarm] <= 0)
	{
		boss->Fall(leftarm);
		boss->leftarm->SetParent(nullptr);
	}
	if (boss->parthp[rightleg] <= 0)
	{
		boss->Fall(rightleg);
		boss->rightleg->SetParent(nullptr);
	}
	if (boss->parthp[leftleg] <= 0)
	{
		boss->Fall(leftleg);
		boss->leftleg->SetParent(nullptr);
	}

	//デバッグ用にパーツに直接ダメージ
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

#pragma region HPバーのサイズ
	/*-------------ボス-------------*/
	float hpSize = (boss1HP_SizeX / boss->hp) * boss->hp;
	boss1HP_Red->SetSize({ hpSize, boss1HP_SizeY});

	char str[256];
	sprintf_s(str, "hpSize : %f", hpSize);
	debugText.PrintDebugText(str, 0, 0, 1);

	/*-------------プレイヤー-------------*/
	playerHP->SetSize({ playerHPX, playerHPY });
	playerHP->SetTextureRect({ playerHPX * (playerMaxHp - player->hp),0 }, { 128, 128 });
	
#pragma endregion

#pragma region 部位の取得
	Capsule RightAramCapsule2(Vector3(-20, 10, 5), Vector3(-20, -30, 5), 10, (0, 255, 255));
	Capsule playerCapsule(player->player->GetPos(), player->player->GetPos() + Vector3(0.0f, -30.0f, 0.0f), 2, (0, 255, 255));
	if (mCollision::testCapsuleCapsule(RightAramCapsule2, playerCapsule))
	{
		debugText.PrintDebugText("syutoku", 0, 90);
		if (boss->parthp[rightarm] <= 0)
		{
			if (input->PressKey(DIK_R))
			{
				//パーツ取得時にプレイヤーの座標とかに合わせる
				boss->rightarm->SetParent(player->player);
				boss->rightarm->SetPos(Vector3(0, -50, +20));
				boss->rightarm->SetRotation(Vector3(90, -15, 180));
				boss->rightarm->SetScale(Vector3(1.0f, 1.0f, 1.0f));
				player->enemyWepon = true;
			}
		}
	}

	if (input->PressKeyTrigger(DIK_P)) {
		audio->SoundStop(audio->xAudio2.Get(), soundData[0]);
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
	XMFLOAT3 rote = player->GetNoAttackRotation();
	XMFLOAT3 pos = player->player->GetPos();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	movement = XMVector3TransformNormal(movement, matRot);

	movement *= XMVECTOR{ -1, -1, -1 };
	if (player->attack == false)
	{
		matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	}

	camera->eye = player->player->GetPos() + movement * XMVECTOR{ 100, 100, 100 };
	camera->eye.y = 20;
	camera->target = player->player->GetPos();
	///camera->target.y = 10.0f;

	camera->SetCam(camera);
	camera->Update();

	boss->Update();



#pragma region デバッグテキスト設定
	//int型からatr型へ変換
	std::ostringstream oss;
	std::ostringstream ass;
	oss << boss->hp;
	debugText.PrintDebugText(oss.str(), 500, 0);
	ass << boss->angle;
	debugText.PrintDebugText(ass.str(), 700, 0);

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
	boss1HP_Black->Draw();
	boss1HP_Red->Draw();
	playerHP->Draw();
	// デバッグテキストの描画
	debugText.DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
