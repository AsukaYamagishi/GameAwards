#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include"ModelDraw.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(ModelDraw* object) {
		this->object3d = object;
	}

	inline ModelDraw* GetObject3d() {
		return object3d;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }
	
	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& info) {
		object3d->OnCollision(info);
	}

	int tag = 0;	//コリジョンマネージャーで、形状タイプとの比較以外の比較用のやつ
protected:
	ModelDraw* object3d = nullptr;
	// 形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};

