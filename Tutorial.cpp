#include "Tutorial.h"
#include <cassert>

using namespace DirectX;

Tutorial::Tutorial()
{
}

Tutorial::~Tutorial()
{
	safe_delete(spriteBG);
	//safe_delete(sprite1);
}

void Tutorial::Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// テクスチャ読み込み(１番にするとよくわからんエラー起こる)
	if (!Object2D::LoadTexture(2, L"Resources/setumei.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Object2D::CreateSprite(2, { 0.0f,0.0f });
	
}

void Tutorial::Update()
{
	//キーボード入力更新
	input->Update();
}

void Tutorial::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Object2D::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Object2D::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	//Object3D::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//object3d->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	//Object3D::PostDraw();
#pragma endregion

#pragma region 3Dモデル描画
// 3Dモデル描画前処理
	//ModelObj::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//model->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	//ModelObj::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Object2D::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる

	//AL3 1-2追加
	//sprite1->Draw();
	//sprite2->Draw();
	
	/// </summary>


	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Object2D::PostDraw();
#pragma endregion
}
