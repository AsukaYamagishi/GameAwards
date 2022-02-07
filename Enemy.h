#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "ModelDraw.h"
#include "ModelManager.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>
class Enemy {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::‚ğÈ—ª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Enemy();
	~Enemy();
	static void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();


public:
	bool isAlive;
	Sphere sphere;

private:
	static DirectXCommon* dxCommon;

	ModelDraw* enemy = nullptr;
	//float move = 5.0f;
	Vector3 pos;



};