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
}

void SceneManager::Update()
{
	//シーン切り替え
	if (keyInput->PressKeyTrigger(DIK_RETURN) && sceneNo == titleScene)
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
	else if (keyInput->PressKeyTrigger(DIK_RETURN) && sceneNo == endScene)
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
}
