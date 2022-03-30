#pragma once
#include"Primitive.h"
#include"Vector3.h"

class mCollision
{
public:

	//���Ƌ�
	static bool testSphereSphere(const mSphere &sphere1, const mSphere &sphere2);

	//���ƒ�����(AABB�����t��)
	static bool testSphereBox(const mSphere &sphere, const Box &box);

	//���ƃJ�v�Z��
	static bool testSphereCapsule(const mSphere &sphere, const Capsule &capsule);

	//�J�v�Z���ƃJ�v�Z��
	static bool testCapsuleCapsule(const Capsule &capsule1, const Capsule &capsule2);

	//�����Ɛ����̋�����2������߂�
	static float sqDistanceSegmentSegment(const Vector3 &p1,const Vector3 &q1, const Vector3 &p2, const Vector3 &q2);

	//�l��͈͓��Ɏ��߂�[low, high]
	static float clamp(float x, float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? high : x;
		return x;
	}
};

