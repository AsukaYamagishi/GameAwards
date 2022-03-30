#pragma once
#include"Primitive.h"
#include"Vector3.h"

class mCollision
{
public:

	//球と球
	static bool testSphereSphere(const mSphere &sphere1, const mSphere &sphere2);

	//球と直方体(AABB制限付き)
	static bool testSphereBox(const mSphere &sphere, const Box &box);

	//球とカプセル
	static bool testSphereCapsule(const mSphere &sphere, const Capsule &capsule);

	//カプセルとカプセル
	static bool testCapsuleCapsule(const Capsule &capsule1, const Capsule &capsule2);

	//線分と線分の距離の2乗を求める
	static float sqDistanceSegmentSegment(const Vector3 &p1,const Vector3 &q1, const Vector3 &p2, const Vector3 &q2);

	//値を範囲内に収める[low, high]
	static float clamp(float x, float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? high : x;
		return x;
	}
};

