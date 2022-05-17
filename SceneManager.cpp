#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio)
{
#pragma region nullptrチェック/代入
	assert(dxCommon);
	assert(keyInput);
	assert(padInput);
	assert(audio);

	this->dxCommon = dxCommon;
	this->keyInput = keyInput;
	this->padInput = padInput;
	this->audio = audio;
#pragma endregion

	//各シーン生成/最初のシーン初期化
	title = new Title();
	title->Init(dxCommon, keyInput, padInput, audio);
	end = new EndGame();
	//end->Init(dxCommon, input, audio);
	game = new GameScene();
	game->Init(dxCommon, keyInput, padInput, audio);
	title->Init(dxCommon, keyInput, padInput, audio);
	//ゲーム開始シーンの設定
	sceneNo = titleScene;

#pragma region Sprite初期設定
	// テクスチャ読み込み
	if (!Sprite::LoadTexture(10, L"Resources/sprite/black.png")) {
		assert(0);
		return;
	}
	//// スプライト生成
	black = Sprite::CreateSprite(10, { 0.0f,0.0f });
	
#pragma endregion

	//シーン遷移変数
	sceneFlag = true;
	alpha = 0;
}

void SceneManager::Update()
{
	//シーン遷移
	
	black->SetColor({ 1,0,0,0.5 });
	black->Update();

	//シーン切り替え
	if ((keyInput->PressKeyTrigger(DIK_RETURN) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && sceneNo == titleScene)
	{		
		game->Finalize();
		game->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = gameScene;
	}
	else if (game->gameEndFlag == true && sceneNo == gameScene)
	{
		end->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = endScene;
	}
	else if ((keyInput->PressKeyTrigger(DIK_RETURN) || padInput->IsPadButtonTrigger(XBOX_INPUT_B)) && sceneNo == endScene)
	{
		title->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = titleScene;
	}

	//ゲームシーンデバッグ用
	if (keyInput->PressKeyTrigger(DIK_M))
	{
		game->Finalize();
		game->Init(dxCommon, keyInput, padInput, audio);
		sceneNo = gameScene;
	}

#pragma region シーンアップデート
	
	if (sceneNo == titleScene) {
		title->Update();
	}
	else if (sceneNo == gameScene){
		game->Update();
	}
	else if (sceneNo == endScene) {
		end->Update();
	}
#pragma endregion

}

void SceneManager::Draw()
{
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

	//選択シーン描画
	if (sceneNo == titleScene) {
		title->Draw();
	}
	else if (sceneNo == gameScene)
	{
		game->Draw();
	}
	else if (sceneNo == endScene) {
		end->Draw(game->winJudeg);
	}

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	//前景スプライト描画
	black->Draw();

	// デバッグテキストの描画
	//debugText.DrawAll(cmdList);
	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
