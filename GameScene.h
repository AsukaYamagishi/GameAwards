#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include <vector>
#include "Player.h"
#include "Collision.h"
#include "FbxDraw.h"
#include "OBJObject.h"
#include"Boss.h"
#include "ParticleManager.h"
#include"Primitive.h"
#include"mCollision.h"

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
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	Camera *camera = nullptr;
	DebugText debugText;
	Sprite *spriteBG;

	//ゲームシーン用
	Sprite *sprite = nullptr;
	//音楽
	Audio::SoundData soundData[3];

	//プレイヤー
	Player *player = nullptr;	
	//武器
	OBJObject *weapon = nullptr;
	//ステージ
	OBJObject *stage = nullptr;
	OBJObject *skydome = nullptr;
	//ボス
	Boss *boss = nullptr;

	//FBXモデル
	FbxModel *testModel = nullptr;
	FbxDraw *testObject = nullptr;

	//パーティクル
	ParticleManager* particleMan = nullptr;


	//敵
	//vector<Enemy*> enemys;
	bool hitFlag[6] = { false };


	Vector3 meye = { 0,0,0 };
	Vector3 mtarget = { 0,0,0 };


public:
	//ゲームシーン終了用
	bool gameEndFlag = false;

#pragma endregion

#pragma region 静的メンバ関数

private: //静的メンバ関数
public: //静的メンバ関数
#pragma endregion

#pragma region ゲッター/セッター

#pragma endregion

#pragma region メンバ関数

private: //メンバ関数
public: //メンバ関数
	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();
	//初期化
	void Init(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio);
	//毎フレーム更新処理
	void Update();
	//描画
	void Draw();
#pragma endregion

};

