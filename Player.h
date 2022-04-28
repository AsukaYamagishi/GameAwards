﻿#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

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
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(Camera camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	Vector3 GetNoAttackRotation() { return player->GetRotation() - attacktorota; }

	//各軸の座標を得る関数
	float GetPosX() { Vector3 pos = player->GetPos(); return pos.x; }
	float GetPosY() { Vector3 pos = player->GetPos(); return pos.y; }
	float GetPosZ() { Vector3 pos = player->GetPos(); return pos.z; }

	//プレイヤーが撃つ球
	ModelDraw* bullet = nullptr;


public:
	ModelDraw* player = nullptr;
	int attacktime = 0;
	bool attack = false;
	bool oldattack = false;
	bool enemyWepon = false;
	int hp = 10;
	bool jumpflag = false;
	float graundheight = 5.0f;//地面の高さ
	bool headFlag = false;
private:
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;

	float move = 1.5f;	
	const float initjumpNum = 2.0f;
	float jumpadd = initjumpNum;
	float gravity = 0;	


	void BeamAttack();
public:
	Vector3 attacktorota = { 0.0f,0.0f,0.0f };

};