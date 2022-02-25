#include "GameScene.h"
#include <cassert>
#include <time.h>

using namespace DirectX;


GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	//safe_deleteはここで行う
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
	/*if (!Sprite::LoadTexture(3, L"Resources/setumei.png")) {
		assert(0);
		return;
	}*/
	//// 背景スプライト生成
	sprite = Sprite::CreateSprite(3, { 0.0f,0.0f });
#pragma endregion


#pragma region 3DモデルCreate・初期設定

#pragma endregion

#pragma region 音楽リソース初期設定

	soundData[0] = audio->SoundLoadWave("Resources/musicloop.wav");
	//audio->SoundPlayWave(audio->xAudio2.Get(), soundData[0], Audio::loop);

#pragma endregion

	player = new Player();
	player->Initialize(dxCommon,input,audio);

	gameEndFlag = false;
}

void GameScene::Update()
{
	player->Update();


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
