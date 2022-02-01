#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio)
{
#pragma region nullptrチェック/代入
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;
#pragma endregion

	//各シーン生成/最初のシーン初期化
	title = new Title();
	title->Init(dxCommon, input, audio);
	tutorial = new Tutorial();
	game = new GameScene();

	//ゲーム開始シーンの設定
	sceneNo = titleScene;
}

void SceneManager::Update()
{
	//シーン切り替え
	if (input->PressKeyTrigger(DIK_SPACE) && sceneNo == titleScene)
	{
		tutorial->Init(dxCommon, input, audio);
		sceneNo = tutorialScene;
	}
	else if (input->PressKeyTrigger(DIK_SPACE) && sceneNo == tutorialScene)
	{
		game->Init(dxCommon, input, audio);
		sceneNo = gameScene;
	}

#pragma region シーンアップデート
	
	if (sceneNo == titleScene) {
		title->Update();
	}
	else if (sceneNo == tutorialScene) {
		tutorial->Update();
	}
	else if (sceneNo == gameScene){
		game->Update();
	}
#pragma endregion

}

void SceneManager::Draw()
{
	//選択シーン描画
	if (sceneNo == titleScene) {
		title->Draw();
	}
	else if (sceneNo == tutorialScene) {
		tutorial->Draw();
	}
	else if (sceneNo == gameScene)
	{
		game->Draw();
	}
}
