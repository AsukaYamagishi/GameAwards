#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "Object3D.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "ModelDraw.h"

using namespace Microsoft::WRL;
using namespace DirectX;

//ゲームシーン
class EndGame
{
#pragma region 変数
private: //静的メンバ変数
	

private: //メンバ変数
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio* audio = nullptr;
	DebugText debugText;

	//ゲームシーン用
	Sprite* spriteBG = nullptr;
	Sprite* sprite1 = nullptr;
	Sprite* number = nullptr;
	Sprite* lank = nullptr;
	Sprite* comma = nullptr;

	//Audio::SoundData soundData[2];
#pragma endregion

#pragma region 関数
public: //メンバ関数
	//コンストラクタ
	EndGame();
	//デストラクタ
	~EndGame();
	//初期化
	void Init(DirectXCommon* dxCommon, KeyboardInput* keyInput, ControllerInput* padInput, Audio* audio);
	//毎フレーム更新処理
	void Update();
	//描画
	void Draw(bool winjude);
#pragma endregion
};

