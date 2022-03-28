#include "mCollision.h"

bool mCollision::testSphereSphere(const mSphere &sphere1, const mSphere &sphere2)
{
	//2つの球の中人座標を結ぶベクトルを求める
	Vector3 vec =
	{ (sphere1.position.x - sphere2.position.x) ,
		(sphere1.position.y - sphere2.position.y),
		(sphere1.position.z - sphere2.position.z) };

	//ベクトルの長さの2乗(2点間の距離の2乗)を求める
	float sqLength = (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	//2つの球の半径の合計を求める
	float r = sphere1.radius + sphere2.radius;

	return sqLength < r *r;
}

bool mCollision::testSphereBox(const mSphere &sphere, const Box &box)
{
	//まず、球の中心座標と AABBとの距離の2乗を求める
	float sqDistance = 0.0f;	//資料スライドでは d2 としている
	float pos;

	//x軸方向の距離の2乗を加算
	pos = sphere.position.x;
	if (pos < box.minPosition.x)
	{
		sqDistance += (box.minPosition.x - sphere.position.x) * (box.minPosition.x - sphere.position.x);
	}
	else if (pos > box.maxPosition.x)
	{
		sqDistance += (sphere.position.x - box.maxPosition.x) * (sphere.position.x - box.maxPosition.x);
	}

	//y軸方向の距離の2乗を加算
	pos = sphere.position.y;
	if (pos < box.minPosition.y)
	{
		sqDistance += (box.minPosition.y - sphere.position.y) * (box.minPosition.y - sphere.position.y);
	}
	else if (pos > box.maxPosition.y)
	{
		sqDistance += (sphere.position.y - box.maxPosition.y) * (sphere.position.y - box.maxPosition.y);
	}

	//z軸方向の距離の2乗を加算
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
	//1. カプセル内の線分のスタート位置からエンド位置へのベクトルを作る
	Vector3 vStartToEnd = capsule.endPosition - capsule.startPosition;

	//2. 1.のベクトルを単位ベクトル化(normalize)し、n　に用意する
	Vector3 n = vStartToEnd;
	n.Normalize();

	//3. Ps -> Pc へのベクトルと 2.で求めた n とのない席を計算する
	//		(すると n を何倍すればベクトル Ps -> Pn になるか倍率(t)が決まる)
	Vector3 vt = sphere.position - capsule.startPosition;
	float t = vt.VDot(n);

	//4. Ps ->Pn ベクトルを求めておく、また、Pnの座標を求めておく
	Vector3 vPsPn = n * t;
	Vector3 posPn = capsule.startPosition + vPsPn;

	//5. 比率 t / (PS -> Pe の長さ)を求める
	float lengthRate = t / vStartToEnd.Length();

	//6. lengthRate < 0 , 0 <= lengthRate <= 1, 1 < lengthRate で場合分け
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
	// 線分と線分の距離を調べる ※結果は距離の2乗の値である
	float sqDistance =
		sqDistanceSegmentSegment(capsule1.startPosition, capsule1.endPosition, capsule2.startPosition, capsule2.endPosition);

	// 2つのカプセルの半径の値を求め
	float r = capsule1.radius + capsule2.radius;

	// 衝突判定の結果を返す
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

	float denominator = a * e - b * b;	//分母

	// s の値を求める
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else
	{
		s = 0.0f;
	}

	// t の値を求める
	// t = ((p1 + d1 * s) - p2)・d2 / (d2・d2)	
	//t = ((p1 + d1 * s) - p2).dot(d2) / e;
	t = (b * s + f) / e;

	// t が[0.0,1.0]の範囲外であれば、s を再計算する
	// s = ((p2 + d2 * t) - p1)・d1 / (d1・d1) = (t * b - c) / a
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
	// s と t の値が決定したので、各線分内の座標 c1 と c2 を求める
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;

	//2点間(c1 と c2) の距離の 2乗を求めて、結果を返す	
	return (((c2.x - c1.x) * (c2.x - c1.x)) + ((c2.y - c1.y) * (c2.y - c1.y)) + ((c2.z - c1.z) * (c2.z - c1.z)));
}
