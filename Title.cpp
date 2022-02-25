#include "Title.h"
#include <cassert>

using namespace DirectX;

Title::Title()
{
}

Title::~Title()
{
	//safe_deleteはここで行う
}

void Title::Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
#pragma region nullptrチェック/代入
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
#pragma endregion

#pragma region Sprite初期設定
	// テクスチャ読み込み(１番にするとよくわからんエラー起こる)
	/*if (!Sprite::LoadTexture(2, L"Resources/title.png")) {
		assert(0);
		return;
	}*/
	//// 背景スプライト生成
	//spriteBG = Sprite::CreateSprite(2, { 0.0f,0.0f });
#pragma endregion


#pragma region 3DモデルCreate・初期設定
#pragma endregion
	
#pragma region 音楽リソース初期設定
#pragma endregion
}

void Title::Update()
{
	//キーボード入力更新
	KeyboardInput::Update();
}

void Title::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// 背景スプライト描画
	spriteBG->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
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
	
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	//Object2D::PreDraw(cmdList);

	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	//Object2D::PostDraw();
#pragma endregion
}
