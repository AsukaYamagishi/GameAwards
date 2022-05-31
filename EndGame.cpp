#include "EndGame.h"
#include <cassert>

using namespace DirectX;

EndGame::EndGame()
{
}

EndGame::~EndGame()
{
	safe_delete(spriteBG);
	//safe_delete(sprite1);
}

void EndGame::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(keyInput);
	assert(padInput);
	assert(audio);

	this->dxCommon = dxCommon;
	this->keyInput = keyInput;
	this->padInput = padInput;
	this->audio = audio;


#pragma region sprite関係
	// テクスチャ読み込み(１番にするとよくわからんエラー起こる)
	if (!Sprite::LoadTexture(2, L"Resources/sprite/clear.png")) {
		assert(0);
		return;
	}
	Sprite::LoadTexture(3, L"Resources/sprite/failure.png");
	Sprite::LoadTexture(30, L"Resources/sprite/number.png");
	//// 背景スプライト生成
	spriteBG = Sprite::CreateSprite(2, { 0.0f,0.0f });
	spriteBG->SetSize({ 1280, 720 });
	spriteBG->Update();

	sprite1 = Sprite::CreateSprite(3, { 0.0f,0.0f });
	sprite1->SetSize({ 1280, 720 });
	sprite1->Update();

	number = Sprite::CreateSprite(30, { 0.0f,0.0f });
	number->Update();
	
#pragma endregion
}

void EndGame::Update()
{
	number->SetSize({ 128, 128 });
	number->SetTextureRect({ 0,0 }, { 32, 32 });
	number->Update();
}

void EndGame::Draw(bool winjude)
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	if (winjude)
	{
		spriteBG->Draw();
	}
	else
	{
		sprite1->Draw();
	}

	number->Draw();
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
// 3Dモデル描画前処理
	//ModelObj::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//model->Draw();

	// 3Dオブジェクト描画後処理
	//ModelObj::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる

	//AL3 1-2追加
	//sprite1->Draw();
	//sprite2->Draw();
	
	

	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
