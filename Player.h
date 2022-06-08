#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"
#include "FbxDraw.h"
#include <DirectXMath.h>
class Player {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::��ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Player();
	~Player();
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, ControllerInput* padInput, Audio* audio);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(Camera camera, Vector3 bossPos, bool cameraFlag);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	Vector3 GetNoAttackRotation() { return player->GetRotation() - attacktorota; }

	//各軸の座標を得る関数
	float GetPosX() { Vector3 pos = player->GetPos(); return pos.x; }
	float GetPosY() { Vector3 pos = player->GetPos(); return pos.y; }
	float GetPosZ() { Vector3 pos = player->GetPos(); return pos.z; }

	//ダメージ処理
	void HitDamege(Vector3 bossPos);

	//ノックバック処理
	void KnockBack(Vector3 bossPos);

	//ジャンプ処理
	void Jump();

	//プレイヤーが撃つ球
	ModelDraw* bullet = nullptr;


public:
	ModelDraw* player = nullptr;
	FbxDraw* playerWalk = nullptr;
	std::unique_ptr<FbxModel> playerWalkModel;
	int attacktime = 0;
	bool attack = false;
	bool oldattack = false;
	bool enemyWepon = false;
	int hp = 10;
	float damageCoolTime = 100.0f;
	bool jumpflag = false;
	bool knockBackFlag = false;
	float graundheight = 5.0f;//地面の高さ
	bool headFlag = false;
	bool isWalk = false;
private:
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio* audio = nullptr;

	float move = 1.5f;	
	const float initjumpNum = 0.5f;
	float jumpadd = initjumpNum;
	float jumpPower;
	float gravity = 0;
	Vector3 jumpPos = { 0, 0, 0 };
	Vector3 oldPlayerPos = { 0, 0, 0 };
	Vector3 oldPlayerPos2 = { 0, 0, 0 };	//エリアないにとどめる用のold変数
	Vector3 testPos = { 0, 0, 0 };
	Vector3 playerFront = { 0, 0, 0 };
	Vector3 bulletPos = { 0, 0, 0 };

	void BeamAttack();

	Vector3 oldRota = { 0,0,0 };
public:
	Vector3 attacktorota = { 0.0f,0.0f,0.0f };

};