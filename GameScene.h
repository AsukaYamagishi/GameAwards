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
#include "PlayerBullet.h"
#include "Enemy.h"
#include "Collision.h"


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
	Sprite* spriteBG;

	//ゲームシーン用
	Sprite* sprite = nullptr;
	//音楽
	Audio::SoundData soundData[3];

	//プレイヤー
	Player* player;
	//プレイヤー弾
	vector<PlayerBullet*> bullets;

	//敵
	vector<Enemy*> enemys;


	//時間経過カウンター
	int counter = 0;
	//ゲームシーン終了用
	int deadEnemyCount = 0;
	


	//壁
	/*ModelDraw* wall[4] = { nullptr };
	enum WALL {
		up,
		down,
		left,
		right
	};*/

public:
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
	void Init(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);
	//毎フレーム更新処理
	void Update();
	//描画
	void Draw();
#pragma endregion

};

