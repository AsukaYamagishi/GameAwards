#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>

enum Parts
{
	head,
	body,
	rightarm,
	leftarm,
	rightleg,
	leftleg
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
	void Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio, ModelDraw* player);

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
	void Attack();

public:
	ModelDraw *boss = nullptr;

	ModelDraw *head = nullptr;
	ModelDraw *body = nullptr;
	ModelDraw *leftarm = nullptr;
	ModelDraw *rightarm = nullptr;
	ModelDraw *leftleg = nullptr;
	ModelDraw *rightleg = nullptr;
	int hp = 18;
	int parthp[6] = { 3,3,3,3,3,3 };
	float angle = 0.0f;
	bool stopFlag = false;

private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	ModelDraw *player = nullptr;
	ModelDraw *bullet = nullptr;
	//プレイヤーへの移動速度の割合
	float move = 128.0f;
	//攻撃用変数
	float coolTime = 100.0f;
	float chargeTime = 20.0f;
	bool attackFlag = false;
};