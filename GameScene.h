#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include <vector>


using namespace Microsoft::WRL;
using namespace DirectX;

//ゲームシーン
class GameScene
{
#pragma region 変数
private: //静的メンバ変数
	//デバッグテキスト用のテクスチャ
	static const int debugTextTexNum = 0;

private: //メンバ変数
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	//ゲームシーン用
	//音楽
	Audio::SoundData soundData[3];

#pragma endregion

#pragma region 静的メンバ関数
	
private: //静的メンバ関数
public: //静的メンバ関数
#pragma endregion

#pragma region ゲッター/セッター
	//a
#pragma endregion

#pragma region メンバ関数

private: //メンバ関数
public: //メンバ関数
	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();
	//初期化
	void Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);
	//毎フレーム更新処理
	void Update();
	//描画
	void Draw();
#pragma endregion

};

