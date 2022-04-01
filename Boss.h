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
	rightaram,
	leftaram,
	rightleg,
	leftleg
};

class Boss {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Boss();
	~Boss();
	void Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio, ModelDraw* player);

	/// <summary>
	/// ñàÉtÉåÅ[ÉÄèàóù
	/// </summary>
	void Update();

	/// <summary>
	/// ï`âÊ
	/// </summary>
	void Draw();

	void HitDamage(int part, int damege = 0);

	void Fall(int part);

	void Move();
	bool AttackRangeJudge(float attackRange);


public:
	ModelDraw *boss = nullptr;

	ModelDraw *head = nullptr;
	ModelDraw *body = nullptr;
	ModelDraw *leftaram = nullptr;
	ModelDraw *rightaram = nullptr;
	ModelDraw *leftleg = nullptr;
	ModelDraw *rightleg = nullptr;
	int hp = 18;
	int parthp[6] = { 3,3,3,3,3,3 };

private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	ModelDraw *player = nullptr;

	float move = 128.0f;
};