#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "ModelDraw.h"
#include "ModelManager.h"
#include "Vector3.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>
class PlayerBullet {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PlayerBullet(Vector3 playerPos);
	~PlayerBullet();
	static void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// ñàÉtÉåÅ[ÉÄèàóù
	/// </summary>
	void Update();

	/// <summary>
	/// ï`âÊ
	/// </summary>
	void Draw();


public:
	bool isAlive;
	Sphere sphere;

private:
	static DirectXCommon* dxCommon;

	ModelDraw* bullet = nullptr;
	const float move = 8.0f;

	Vector3 pos = { 0,0,0 };
	
	Vector3 startPos = { 0,0,0 };



};