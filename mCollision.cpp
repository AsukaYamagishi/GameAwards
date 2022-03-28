#include "mCollision.h"

bool mCollision::testSphereSphere(const mSphere &sphere1, const mSphere &sphere2)
{
	//2�̋��̒��l���W�����ԃx�N�g�������߂�
	Vector3 vec =
	{ (sphere1.position.x - sphere2.position.x) ,
		(sphere1.position.y - sphere2.position.y),
		(sphere1.position.z - sphere2.position.z) };

	//�x�N�g���̒�����2��(2�_�Ԃ̋�����2��)�����߂�
	float sqLength = (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	//2�̋��̔��a�̍��v�����߂�
	float r = sphere1.radius + sphere2.radius;

	return sqLength < r *r;
}

bool mCollision::testSphereBox(const mSphere &sphere, const Box &box)
{
	//�܂��A���̒��S���W�� AABB�Ƃ̋�����2������߂�
	float sqDistance = 0.0f;	//�����X���C�h�ł� d2 �Ƃ��Ă���
	float pos;

	//x�������̋�����2������Z
	pos = sphere.position.x;
	if (pos < box.minPosition.x)
	{
		sqDistance += (box.minPosition.x - sphere.position.x) * (box.minPosition.x - sphere.position.x);
	}
	else if (pos > box.maxPosition.x)
	{
		sqDistance += (sphere.position.x - box.maxPosition.x) * (sphere.position.x - box.maxPosition.x);
	}

	//y�������̋�����2������Z
	pos = sphere.position.y;
	if (pos < box.minPosition.y)
	{
		sqDistance += (box.minPosition.y - sphere.position.y) * (box.minPosition.y - sphere.position.y);
	}
	else if (pos > box.maxPosition.y)
	{
		sqDistance += (sphere.position.y - box.maxPosition.y) * (sphere.position.y - box.maxPosition.y);
	}

	//z�������̋�����2������Z
	pos = sphere.position.z;
	if (pos < box.minPosition.z)
	{
		sqDistance += (box.minPosition.z - sphere.position.z) * (box.minPosition.z - sphere.position.z);
	}
	else if (pos > box.maxPosition.z)
	{
		sqDistance += (sphere.position.z - box.maxPosition.z) * (sphere.position.z - box.maxPosition.z);
	}

	return sqDistance < sphere.radius *sphere.radius;
}

bool mCollision::testSphereCapsule(const mSphere &sphere, const Capsule &capsule)
{
	//1. �J�v�Z�����̐����̃X�^�[�g�ʒu����G���h�ʒu�ւ̃x�N�g�������
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	//2. 1.�̃x�N�g����P�ʃx�N�g����(normalize)���An�@�ɗp�ӂ���
	Vector3 n = vStartToEnd;
	n.Normalize();

	//3. Ps -> Pc �ւ̃x�N�g���� 2.�ŋ��߂� n �Ƃ̂Ȃ��Ȃ��v�Z����
	//		(����� n �����{����΃x�N�g�� Ps -> Pn �ɂȂ邩�{��(t)�����܂�)
	Vector3 vt = sphere.position - capsule.startPosition;
	float t = vt.VDot(n);

	//4. Ps ->Pn �x�N�g�������߂Ă����A�܂��APn�̍��W�����߂Ă���
	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition + vPsPn;

	//5. �䗦 t / (PS -> Pe �̒���)�����߂�
	float lengthRate = t / vStartToEnd.Length();

	//6. lengthRate < 0 , 0 <= lengthRate <= 1, 1 < lengthRate �ŏꍇ����
	float distance;
	Vector3 vdistance;
	if (lengthRate < 0.0f)
	{
		vdistance = sphere.position - capsule.startPosition;
		distance = vdistance.Length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		vdistance = sphere.position - posPn;
		distance = vdistance.Length() - capsule.radius;
	}
	else
	{
		vdistance = sphere.position - capsule.endPosition;
		distance = vdistance.Length() - capsule.radius;
	}

	return distance < sphere.radius;
}

bool mCollision::testCapsuleCapsule(const Capsule &capsule1, const Capsule &capsule2)
{
	// �����Ɛ����̋����𒲂ׂ� �����ʂ͋�����2��̒l�ł���
	float sqDistance =
		sqDistanceSegmentSegment(capsule1.startPosition, capsule1.endPosition, capsule2.startPosition, capsule2.endPosition);

	// 2�̃J�v�Z���̔��a�̒l������
	float r = capsule1.radius + capsule2.radius;

	// �Փ˔���̌��ʂ�Ԃ�
	return sqDistance < r *r;
}

float mCollision::sqDistanceSegmentSegment(const Vector3 &p1, const Vector3 &q1, const Vector3 &p2, const Vector3 &q2)
{
	Vector3 d1 = q1 - p1;
	Vector3 d2 = q2 - p2;

	Vector3 r = p1 - p2;

	float a = d1.VDot(d1);
	float b = d1.VDot(d2);
	float e = d2.VDot(d2);

	float c = d1.VDot(r);
	float f = d2.VDot(r);

	float s = 0.0f;
	float t = 0.0f;

	float denominator = a * e - b * b;	//����

	// s �̒l�����߂�
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	// t �̒l�����߂�
	// t = ((p1 + d1 * s) - p2)�Ed2 / (d2�Ed2)	
	//t = ((p1 + d1 * s) - p2).dot(d2) / e;
	t = (b * s + f) / e;

	// t ��[0.0,1.0]�͈̔͊O�ł���΁As ���Čv�Z����
	// s = ((p2 + d2 * t) - p1)�Ed1 / (d1�Ed1) = (t * b - c) / a
	if (t < 0.0f)
	{
		t = 0.0f;
		s = clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		s = clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	// s �� t �̒l�����肵���̂ŁA�e�������̍��W c1 �� c2 �����߂�
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;

	//2�_��(c1 �� c2) �̋����� 2������߂āA���ʂ�Ԃ�	
	return (((c2.x - c1.x) * (c2.x - c1.x)) + ((c2.y - c1.y) * (c2.y - c1.y)) + ((c2.z - c1.z) * (c2.z - c1.z)));
}
