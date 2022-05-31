#pragma once
#include "GameScene.h"
#include "Title.h"
#include "EndGame.h"

class SceneManager
{
#pragma region 変数
private: //メンバ変数
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio* audio = nullptr;
	GameScene* game = nullptr;
	Title* title = nullptr;
	EndGame* end = nullptr;
	Audio::SoundData soundData[2];


	Sprite* black = nullptr;
	Sprite* enemy = nullptr;
	Sprite* enemy_White = nullptr;

	//シーン遷移番号
	enum SceneNo {
		titleScene,
		gameScene,
		endScene
	};
	SceneNo sceneNo;

#pragma endregion

#pragma region 静的メンバ関数

private: //静的メンバ関数
public: //静的メンバ関数
#pragma endregion

#pragma region ゲッター/セッター
	//a
#pragma endregion

#pragma region メンバ関数
public: //メンバ関数
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();
	//初期化
	void Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio);
	//毎フレーム更新処理
	void Update();
	//描画
	void Draw();
#pragma endregion
private:
	bool result=false;
	float alpha;
	bool titleGameFlag;
	bool gameEndFlag;

	//アニメーション
	float enemyTimer;
	float enemyMaxTimer;
	float i;
	float count;
};

