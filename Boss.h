#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>
#include"FbxDraw.h"
enum Parts
{
	head,
	body,
	rightarm,
	leftarm,
	rightleg,
	leftleg
};

enum Sound
{
	Charge = 0,
	Shot
};

enum AttackType {
	NONE,
	BEAM,
	PRESS,
	RUSH
};

class Boss {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Boss();
	~Boss();
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio, ModelDraw* player);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//攻撃ヒット時のダメージ
	void HitDamage(int part, int damege = 0);

	//部位が落ちる処理
	void Fall(int part);

	//ボスの移動処理
	void Move();

	//ボスの向きをプレイヤーに向くようにする処理
	void Direction();

	//プレイヤーの位置がボスの行動(移動や攻撃)の範囲内か判定する処理
	bool RangeJudge(float attackRange);

	//ボスの攻撃処理
	//ビーム攻撃
	void BeamAttack();

	//プレス攻撃
	void PressAttack();

	//突進攻撃
	void Rush();

	//ダメージ処理
	int damage(float weaponATK);

	//位置補正
	void PosCorrection();

	bool GetAttackFlag() { return attackFlag; }
	int GetAttackTime() { return attackTime; }
	int GetAttackType() { return attackType; }
public:
	ModelDraw* boss = nullptr;

	ModelDraw* head = nullptr;
	ModelDraw* body = nullptr;
	ModelDraw* leftarm = nullptr;
	ModelDraw* rightarm = nullptr;
	ModelDraw* leftleg = nullptr;
	ModelDraw* rightleg = nullptr;
	
	

	FbxDraw* fbxparantboss = nullptr;

	//FbxDraw* head = nullptr;
	//FbxDraw* body = nullptr;
	FbxDraw* fbxRightArm = nullptr;
	FbxDraw* fbxLeftArm = nullptr;
	FbxDraw* fbxLeftLeg = nullptr;
	FbxDraw* fbxRightLeg = nullptr;
	FbxDraw* fbxLeftArmWalk = nullptr;
	FbxDraw* fbxRightArmWalk = nullptr;
	FbxDraw* fbxLeftArmPress = nullptr;
	FbxDraw* fbxRightArmPress = nullptr;

	//std::unique_ptr<FbxModel> headModel;
	//std::unique_ptr<FbxModel> bodyModel;
	std::unique_ptr<FbxModel> fbxRightArmModel;
	std::unique_ptr<FbxModel> fbxLeftArmModel;
	std::unique_ptr<FbxModel> fbxRightLegModel;
	std::unique_ptr<FbxModel> fbxLeftLegModel;

	std::unique_ptr<FbxModel> fbxRightArmWarkModel;
	std::unique_ptr<FbxModel> fbxLeftArmWarkModel;
	std::unique_ptr<FbxModel> fbxRightArmPressModel;
	std::unique_ptr<FbxModel> fbxLeftArmPressModel;


	const int maxhp = 18;
	int hp = maxhp;
	int parthp[6] = { 3,3,3,3,3,3 };
	float angle = 0.0f;
	float watch = 0.0f;
	float watchB = 0.0f;
	bool stopFlag = false;
	int attackType = NONE;

	int timer = 0;
private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	ModelDraw *player = nullptr;
	ModelDraw *bullet = nullptr;
	ModelDraw* shockWave = nullptr;
	//プレイヤーへの移動速度の割合
	float move = 128.0f;

	//攻撃用変数
	//共通変数
	Vector3 oldBossPos = { 0, 0, 0 };
	Vector3 oldPlayerPos = { 0, 0, 0 };
	Vector3 correctionPos = { 0, 0, 0 };
	float coolTime = 100.0f;
	float chargeTime = 30.0f;
	bool attackFlag = false;
	float shakePosX = 0.0f;
	float shakePosZ = 0.0f;
	bool noneLeg = false;
	bool noneArm = false;
	bool noneLeftArm = false;
	bool noneRightArm = false;
	//ビーム攻撃用変数
	Vector3 bulletPos = { 0, 0, 0 };
	Vector3 bulletScale = { 0, 0, 0 };
	float attackTime = 100.0f;
	//プレス攻撃用変数
	Vector3 pressPos = { 0, 0, 0 };
	Vector3 shockPos = { 0, 0, 0 };
	Vector3 shockScale = { 0, 0, 0 };
	float pressPower = 0.0f;
	float pressWaitTime = 10.0f;
	bool shockFlag = false;
	bool pressWaitFlag = false;
	//突進攻撃用変数
	int rushCount = 0;
	
	//SE用変数
	Audio::SoundData soundSE[2];
};