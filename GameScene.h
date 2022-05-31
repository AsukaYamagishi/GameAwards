#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include <vector>
#include "Player.h"
#include "Collision.h"
#include "FbxDraw.h"
#include "OBJObject.h"
#include"Boss.h"
#include "ParticleManager.h"
#include"Primitive.h"
#include "mCollision.h"
#include"Effects.h"
class CollisionManager;
class FbxCollisionManager;
#include"Weapon.h"
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
	KeyboardInput *keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio *audio = nullptr;
	Camera *camera = nullptr;
	DebugText debugText;
	Sprite *spriteBG;

	//ゲームシーン用
	float boss1HP_SizeX = 800;
	float boss1HP_SizeY = 32;
	Sprite* boss1HP_Red = nullptr;
	Sprite* boss1HP_Black = nullptr;
	float playerHPX = 200;
	float playerHPY = 200;
	int playerMaxHp = 10;
	Sprite* playerHP = nullptr;
	Sprite* controler_rule = nullptr;
	Sprite* ketboard_rule = nullptr;
	Sprite* pose = nullptr;
	Sprite* pose_key = nullptr;
	bool poseFlag = false;
	//拾える武器用矢印
	ModelDraw* arrow[5];
	bool DrawFlag[5];
	XMFLOAT3 arrowPos[5];
	float frame;
	float downTimer[5];
	bool isKeyBoard = false;
	bool isGamePad = true;
	//音楽
	Audio::SoundData soundData[2];
	Audio::SoundData soundSE[7];

	//コリジョンマネージャーに送ってどのパーツ、対象が当たっているのかを保存する
	bool hit[20] = { false };
	//ゲームシーンに存在するアタックする奴らのアタックフラグを全部入れる
	bool attackFlag[10] = { false };
	//プレイヤー
	Player *player;	
	//武器
	//OBJObject *weapon;
	Weapon* weapon = nullptr;
	//ステージ
	OBJObject *stage;
	OBJObject *skydome;
	//ボス
	Boss *boss;

	//衝突マネージャ
	CollisionManager* collisionManager = nullptr;
	FbxCollisionManager* fbxcollisionManager = nullptr;

	//FBXモデル
	std::unique_ptr<FbxModel> testModel = nullptr;
	FbxDraw *testObject = nullptr;
	std::unique_ptr<FbxModel> testsphereModel = nullptr;
	FbxDraw* testsphereObject = nullptr;

	//パーティクル
	ParticleManager* particleMan = nullptr;

	/*std::unique_ptr<Effects>*/Effects* effects = nullptr;
	Effects* effects_2 = nullptr;

	bool isfirework = false;
	bool isexplosion = false;

	//敵
	//vector<Enemy*> enemys;

	float angle = 3.14f / 2.0f;

	float radius = XMConvertToRadians(2.0f);
	int soundNo = 0;
	int seNo = 0;

	int pressTimer = 0;
	float pressRadius = 0;
	bool fireFlag = false;
	float fireTimer = 0;

	bool fallFlag[6] = { 0 };

	bool fallFlag[6] = { 0 };

	bool LockFlag = false;
	bool cameraFlag = false;
	float cameraAngle;

	bool bgmflag = false;
public:
	//ゲームシーン終了用
	bool gameFlag = false;
	bool winJudeg = false;
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

	//
	void Finalize();

	//初期化
	void Init(DirectXCommon *dxCommon, KeyboardInput *keyInput, ControllerInput *padInput, Audio *audio);
	//毎フレーム更新処理
	bool Update();
	//描画
	void Draw();
#pragma endregion

};

