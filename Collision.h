#pragma once
///<summary>
///当たり判定ヘルパークラス
///</summary>

#include <DirectXMath.h>
#include "CollisionPrimitive.h"
#include "Vector3.h"

class Collision
{
public:
	///<summary>
    ///球と平面の当たり判定
    ///</summary>
	///<param name="sphere">球</param>
	///<param name="plane">平面</param>
	///<param name="inter">交点(平面上の最近接点)</param>
	///<returns>交差しているか否か</returns>
	static bool IsSphereToPlane(
		const Sphere &sphere,
		const Plane &plane,
		DirectX::XMVECTOR *inter = nullptr
	);

	///<summary>
	///点と三角形の最近接点を求める
	///</summary>
	///<param name="point">点</param>
	///<param name="triangle">三角形</param>
	///<param name="closest">最近接点(出力用)</param>
	static void ClossPointToTriangle(
		const DirectX::XMVECTOR &point,
		const Triangle &triangle,
		DirectX::XMVECTOR *closest
	);

	///<summary>
	///球と法線付き三角形の当たり判定
	///</summary>
	///<param name="sphere">球</param>
	///<param name="triangle">三角形</param>
	///<param name="inter">交点(三角形上の最近接点)</param>
	///<returns>交差しているか否か</returns>
	static bool IsSqhereToTriangle(
		const Sphere &sphere,
		const Triangle &triangle,
		DirectX::XMVECTOR *inter = nullptr
	);

	//レイと平面
	static bool IsRayToPlane(
		const Ray &ray,
		const Plane &plane,
		float *distance = nullptr,
		DirectX::XMVECTOR *inter = nullptr
	);

	//レイと法線付き三角
	static bool IsRayToTriangle(
		const Ray &ray,
		const Triangle &triangle,
		float *distance = nullptr,
		DirectX::XMVECTOR *inter = nullptr
	);

	//レイと球
	static bool IsRayToSqhere(
		const Ray &ray,
		const Sphere &sphere,
		float *distance = nullptr,
		DirectX::XMVECTOR *inter = nullptr
	);

    // 球体同士の当たり判定
	static bool IsBallToBallCollision(const Sphere& a,const Sphere& b);
//
//	// 直方体同士の当たり判定
//	bool IsBoxToBoxCollision(const XMVECTOR& pos1, const XMVECTOR& rotation1, const XMVECTOR& scale1, const XMVECTOR& pos2, const XMVECTOR& rotation2, const XMVECTOR& scale2);
//
//
//
//	// 球体スイープ（移動する球同士の判定）
//    // 当たっていたら衝突した時間を0~1の間で返す、当たっていなかったら-1を返す
//	bool sphereSwept(const XMVECTOR& pos1, const XMVECTOR& speed1, const float& r1, const XMVECTOR& pos2, const XMVECTOR& speed2, const float& r2,
//	float &outSecond,	//進行度格納用ポインター
//	XMVECTOR &CollisionPos,	//衝突位置(接点)
//	XMVECTOR *CollisionPosA = 0,	//一つ目が衝突した時の位置
//	XMVECTOR *CollisionPosB = 0	//二つ目が衝突した時の位置
//);
//
//	// 衝突予測
//    // 箱の場合は、XMVECTOR(横幅, 高さ, 奥行).Length() / 2を入れれば使えます
//	bool IsPredictCollisionBall(const XMVECTOR& pos1, const XMVECTOR& speed1, const float& r1, const XMVECTOR& pos2, const XMVECTOR& speed2, const float& r2);

};
