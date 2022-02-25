#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Audio.h"
#include "KeyboardInput.h"
#include "Sprite.h"

using namespace Microsoft::WRL;
using namespace DirectX;

//ゲームシーン
class Title
{
#pragma region 変数
private: //静的メンバ変数
	

private: //メンバ変数
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;
	
	//ゲームシーン用
	Sprite* spriteBG = nullptr;

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
	Title();
	//デストラクタ
	~Title();
	//初期化
	void Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);
	//毎フレーム更新処理
	void Update();
	//描画
	void Draw();
#pragma endregion
};
