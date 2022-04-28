#pragma once

/// <summary>
/// 衝突判定用関連の型定義
/// </summary>

enum CollisionShapeType
{
	SHAPE_UNKNOWN = -1, // 未設定

	COLLISIONSHAPE_SPHERE, // 球
	COLLISIONSHAPE_MESH,	//メッシュ
};

enum CollisionTag
{
	TagNone = 0,
	TagHead = 1,
	TagBody = 2,
	TagRightArm = 3,
	TagLeftArm = 4,
	TagRightLeg = 5,
	TagLeftLeg = 6,
	TagPlayer = 7,
	TagWeapon = 8,
	TagBullet = 9,
};

enum AnyHit
{
	WwaponToHead = 1,
	WwaponToBody,
	WwaponToRightArm,
	WwaponToLeftArm,
	WwaponToRightLeg,
	WwaponToLeftLeg,

	BulletToPlayer,
};