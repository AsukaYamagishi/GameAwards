#include "GameScene.h"
#include <cassert>
#include <time.h>

using namespace DirectX;


GameScene::GameScene()
{
	/*for (int i = 0; i < 4; i++)
	{
		wall[i] = ModelDraw::Create();
	}
	wall[up]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WallUD));
	wall[down]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WallUD));
	wall[left]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WallLR));
	wall[right]->SetModel(ModelManager::GetIns()->GetModel(ModelManager::WallLR));*/
}

GameScene::~GameScene()
{
	//safe_deleteはここで行う
	audio->SoundStop(audio->xAudio2.Get(), soundData[0]);
}

void GameScene::Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
#pragma region nullptrチェック/代入
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
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
	if (!Sprite::LoadTexture(3, L"Resources/setumei.png")) {
		assert(0);
		return;
	}
	//// 背景スプライト生成
	sprite = Sprite::CreateSprite(3, { 0.0f,0.0f });
#pragma endregion


#pragma region 3DモデルCreate・初期設定
	//for (int i = 0; i < 4; i++)
	//{
	//	wall[i] = new ModelDraw();
	//}
	//
	//wall[up]->SetPos({ 0.0f,25.0f,0.0f });
	//wall[up]->SetRotation({ 0.0f,0.0f,90.0f });
	//wall[up]->SetScale({ 2.5f,14.0f,1.0f });
	////wall[up]->Update();
	//
	//wall[down]->SetPos({ 0.0f,-25.0f,0.0f });
	//wall[down]->SetRotation({ 0.0f,0.0f,90.0f });
	//wall[down]->SetScale({ 2.5f,14.0f,1.0f });
	////wall[down]->Update();
	//wall[left]->SetPos({ -37.0f,0.0f,0.0f });
	//wall[left]->SetRotation({ 0.0f,0.0f,0.0f });
	//wall[left]->SetScale({ 15.0f,3.5f,1.0f });
	////wall[left]->Update();
	//wall[right]->SetPos({ 37.0f,0.0f,0.0f });
	//wall[right]->SetRotation({ 0.0f,0.0f,0.0f });
	//wall[right]->SetScale({ 15.0f,3.5f,1.0f });
	////wall[right]->Update();

#pragma endregion

#pragma region 音楽リソース初期設定

	soundData[0] = audio->SoundLoadWave("Resources/musicloop.wav");
	audio->SoundPlayWave(audio->xAudio2.Get(), soundData[0], Audio::loop);

#pragma endregion

	player = new Player();
	player->Initialize(dxCommon,input,audio);

	PlayerBullet::Initialize(dxCommon);
	
	Enemy::Initialize(dxCommon);

	counter = 0;
	deadEnemyCount = 0;
	gameEndFlag = false;
}

void GameScene::Update()
{
	/*for (int i = 0; i < 4; i++)
	{
		wall[i]->Update();
	}*/
	//wall[right]->Update();
	counter++;
	if (counter % 60 == 0)
	{
		enemys.push_back(new Enemy());
	}


	if (KeyboardInput::PressKeyTrigger(DIK_SPACE))
	{
		bullets.push_back(new PlayerBullet(player->player->GetPos()));
	}
	

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Update();
	}
	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Update();
	}

	for (int enemyNum = 0; enemyNum < enemys.size(); enemyNum++)
	{
		for (int bulletNum = 0; bulletNum < bullets.size(); bulletNum++)
		{
			if (Collision::IsBallToBallCollision(enemys[enemyNum]->sphere, bullets[bulletNum]->sphere))
			{
				enemys[enemyNum]->isAlive = false;
				bullets[bulletNum]->isAlive = false;
				deadEnemyCount++;
			}
		}
	}


	for (int i = bullets.size() - 1; i >= 0; i--)
	{
		if (bullets[i]->isAlive == false)
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}

	
	for (int i = enemys.size() - 1; i >= 0; i--)
	{
		if (enemys[i]->isAlive == false)
		{
			delete enemys[i];
			enemys.erase(enemys.begin() + i);
		}
	}

	player->Update();

	if (deadEnemyCount > 15)
	{
 		audio->SoundStop(audio->xAudio2.Get(), soundData[0]);
		//audio->SoundUnLoad(soundData);
		gameEndFlag = true;
	}

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
	/*ModelDraw::PreDraw(cmdList);
	for (int i = 0; i < 4; i++)
	{
		wall[i]->Draw();
	}
	ModelDraw::PostDraw();*/

	player->Draw();
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Draw();
	}
	for (int i = 0; i < enemys.size(); i++)
	{
		enemys[i]->Draw();
	}
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//前景スプライト描画
	sprite->Draw();
	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
