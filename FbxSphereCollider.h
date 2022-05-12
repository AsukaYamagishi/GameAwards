#pragma once
#pragma once

#include "FbxBaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class FbxSphereCollider : public FbxBaseCollider, public Sphere
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	FbxSphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 0.0f) :
		offset(offset),
		radius(radius)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	inline void SetRadius(float radius) { this->radius = radius; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float radius;
};

