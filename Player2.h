﻿#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>

class Player2 : public ModelDraw
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::��ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Player2();
	~Player2();

	static Player2* Create(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);

	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);

	void Update(Camera camera);

	void Draw();

	Vector3 GetNoAttackRotation() { return GetRotation() - attacktorota; }

public:
	//ModelDraw* player = nullptr;
	int attacktime = 0;
	bool attack = false;
	bool oldattack = false;
	bool enemyWepon = false;
	int hp = 10;

private:
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;

	float move = 1.5f;
	bool jumpflag = false;
	const float initjumpNum = 2.0f;
	float jumpadd = initjumpNum;
	float gravity = 0;
	float graundheight = 5.0f;//地面の高さ

public:
	Vector3 attacktorota = { 0.0f,0.0f,0.0f };
};
