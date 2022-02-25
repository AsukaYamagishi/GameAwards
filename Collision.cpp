#include "Collision.h"
#include <DirectXMath.h>

using namespace DirectX;

static XMVECTOR distance{}; //二点間の距離(Collision namespace内共通)

bool Collision::IsSphereToPlane(const Sphere &sphere, const Plane &plane, XMVECTOR *inter)
{
	//座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.centor,plane.normal);
	//平面の原点距離を原産することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	//距離の絶対値が半径よりも大きければ当たっていない
	if (fabsf(dist) > sphere.radius) {
		return false;
	}
	//疑似交点を計算
	if (inter) {
		//平面上の最近接点を疑似交点とする
		*inter = -dist * plane.normal + sphere.centor;
	}
	return true;
}

void Collision::ClossPointToTriangle(const XMVECTOR &point, const Triangle &triangle, XMVECTOR *closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1,p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2,p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return ;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1,p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2,p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1,p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2,p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::IsSqhereToTriangle(const Sphere &sphere, const Triangle &triangle, XMVECTOR *inter) {
	XMVECTOR p;
	//球の中心に対する最近接点である三角形上にある点pを見つける
	ClossPointToTriangle(sphere.centor, triangle, &p);
	//点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.centor;
	//距離の二乗を求める
	//（同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	v = XMVector3Dot(v, v);
	//球と三角形の距離が半径以下なら当たっていない
	if (v.m128_f32[0] > sphere.radius * sphere.radius) {
		return false;
	}
	//疑似交点を計算
	if (inter) {
		//三角形上の最近接点pを疑似交点とする
		*inter = p;
	}
	return true;
}

bool Collision::IsRayToPlane(const Ray &ray, const Plane &plane, float *distance, XMVECTOR *inter)
{
	const float epsilon = 1.0e-5f;	//誤差吸収用
	float d1 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//裏面には当たらない
	if (d1 > -epsilon) {
		return false;
	}
	//面法線と霊の始点座標の内積
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//始点と平面の距離（平面の法線方向）
	float dist = d2 - plane.distance;
	//始点と平面の距離（レイ方向）
	float t = dist / -d1;
	//交点が始点より後ろにあるので当たらない
	if (t < 0) {
		return false;
	}
	//距離を書き込む
	if (distance) {
		*distance = t;
	}
	//交点を計算
	if (inter) {
		*inter = ray.start + (ray.dir * t);
	}
	return true;
}

bool Collision::IsRayToTriangle(const Ray &ray, const Triangle &triangle, float *distance, XMVECTOR *inter)
{
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//レイと平面が当たっていなければ当たっていない
	if (!IsRayToPlane(ray, plane, distance, &interPlane)) {
		return false;
	}
	
	//レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;
	XMVECTOR m;
	
	//辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//辺の外側であれば当たっていない
	if (XMVector3Dot(m,triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	//辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//辺の外側であれば当たっていない
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	//辺p2_p0について
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//辺の外側であれば当たっていない
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	if (inter) {
		*inter = interPlane;
	}
	return true;

}

bool Collision::IsRayToSqhere(const Ray &ray, const Sphere &sphere, float *distance, XMVECTOR *inter) {
	XMVECTOR m = ray.start - sphere.centor;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;

	if (c > 0.0f && b > 0.0f) {
		return false;
	}
	float discr = b * b - c;
	//負の判定式は例が球を外れていることと一致
	if (discr < 0.0f) {
		return false;
	}

	//レイと球は交差している。
	float t = -b - sqrtf(discr);
	//tが負である場合、レイは球の内側から開始しているので、tを0に
	if (t < 0) {
		t = 0.0f;
	}
	if (distance) {
		*distance = t;
	}
	if (inter) {
		*inter = ray.start + ray.dir * t;
	}
	return true;
}

	bool Collision::IsBallToBallCollision(const Sphere& a, const Sphere& b)
	{
		// 中心点間の距離
		Vector3 distance = a.centor - b.centor;

		return distance.Length() < (a.radius + b.radius);
	}
//
//bool Collision::IsBoxToBoxCollision(const XMVECTOR& pos1, const XMVECTOR& rotation1, const XMVECTOR& scale1, const XMVECTOR& pos2, const XMVECTOR& rotation2, const XMVECTOR& scale2)
//{
//	using namespace DirectX;
//
//	// 回転行列
//	static XMMATRIX matRot1{}, matRot2{};
//	// 中心から面までの長さ
//	static XMVECTOR length1{}, length2{};
//	// 分離軸ベクトル
//	static XMVECTOR direction1_XN{}, direction1_YN{}, direction1_ZN{}, direction2_XN{}, direction2_YN{}, direction2_ZN{};
//	// 方向ベクトル
//	static XMVECTOR direction1_X{}, direction1_Y{}, direction1_Z{}, direction2_X{}, direction2_Y{}, direction2_Z{};
//
//	/*OBBの当たり判定の下準備*/
//
//	matRot1 = XMMatrixIdentity();
//	matRot1 *= XMMatrixRotationZ(XMConvertToRadians(rotation1.z));
//	matRot1 *= XMMatrixRotationX(XMConvertToRadians(rotation1.x));
//	matRot1 *= XMMatrixRotationY(XMConvertToRadians(rotation1.y));
//
//	length1 = scale1 / 2.0f;
//	direction1_XN = XMVector4Normalize(matRot1.r[0]);
//	direction1_YN = XMVector4Normalize(matRot1.r[1]);
//	direction1_ZN = XMVector4Normalize(matRot1.r[2]);
//	direction1_X = direction1_XN * length1.x;
//	direction1_Y = direction1_YN * length1.y;
//	direction1_Z = direction1_ZN * length1.z;
//
//	matRot2 = XMMatrixIdentity();
//	matRot2 *= XMMatrixRotationZ(XMConvertToRadians(rotation2.z));
//	matRot2 *= XMMatrixRotationX(XMConvertToRadians(rotation2.x));
//	matRot2 *= XMMatrixRotationY(XMConvertToRadians(rotation2.y));
//
//	length2 = scale2 / 2.0f;
//	direction2_XN = XMVector4Normalize(matRot2.r[0]);
//	direction2_YN = XMVector4Normalize(matRot2.r[1]);
//	direction2_ZN = XMVector4Normalize(matRot2.r[2]);
//	direction2_X = direction2_XN * length2.x;
//	direction2_Y = direction2_YN * length2.y;
//	direction2_Z = direction2_ZN * length2.z;
//
//	/*OBBの当たり判定*/
//
//	distance = pos1 - pos2;
//
//	static float r1{}, r2{}; //半投影線分
//	static float l{}; //中間点間の距離
//
//	// 分離軸ベクトル：direction1_XN
//	r1 = direction1_X.Length();
//	r2 = fabsf(direction1_XN.VDot(direction2_X)) + fabsf(direction1_XN.VDot(direction2_Y)) + fabsf(direction1_XN.VDot(direction2_Z));
//	l = fabsf(direction1_XN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：direction1_YN
//	r1 = direction1_Y.Length();
//	r2 = fabsf(direction1_YN.VDot(direction2_X)) + fabsf(direction1_YN.VDot(direction2_Y)) + fabsf(direction1_YN.VDot(direction2_Z));
//	l = fabsf(direction1_YN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：direction1_ZN
//	r1 = direction1_Z.Length();
//	r2 = fabsf(direction1_ZN.VDot(direction2_X)) + fabsf(direction1_ZN.VDot(direction2_Y)) + fabsf(direction1_ZN.VDot(direction2_Z));
//	l = fabsf(direction1_ZN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：direction2_XN
//	r1 = direction2_X.Length();
//	r2 = fabsf(direction2_XN.VDot(direction1_X)) + fabsf(direction2_XN.VDot(direction1_Y)) + fabsf(direction2_XN.VDot(direction1_Z));
//	l = fabsf(direction2_XN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：direction2_YN
//	r1 = direction2_Y.Length();
//	r2 = fabsf(direction2_YN.VDot(direction1_X)) + fabsf(direction2_YN.VDot(direction1_Y)) + fabsf(direction2_YN.VDot(direction1_Z));
//	l = fabsf(direction2_YN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：direction2_ZN
//	r1 = direction2_Z.Length();
//	r2 = fabsf(direction2_ZN.VDot(direction1_X)) + fabsf(direction2_ZN.VDot(direction1_Y)) + fabsf(direction2_ZN.VDot(direction1_Z));
//	l = fabsf(direction2_ZN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	static XMVECTOR crossDirection{}; //外積分離軸
//
//	// 分離軸ベクトル：Cross(direction1_X, direction2_X)
//	crossDirection = direction1_X.VCross(direction2_X);
//	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_X, direction2_Y)
//	crossDirection = direction1_X.VCross(direction2_Y);
//	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_X, direction2_Z)
//	crossDirection = direction1_X.VCross(direction2_Z);
//	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_Y, direction2_X)
//	crossDirection = direction1_Y.VCross(direction2_X);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_Y, direction2_Y)
//	crossDirection = direction1_Y.VCross(direction2_Y);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_Y, direction2_Z)
//	crossDirection = direction1_Y.VCross(direction2_Z);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_Z, direction2_X)
//	crossDirection = direction1_Z.VCross(direction2_X);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
//	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_Z, direction2_Y)
//	crossDirection = direction1_Z.VCross(direction2_Y);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// 分離軸ベクトル：Cross(direction1_Z, direction2_Z)
//	crossDirection = direction1_Z.VCross(direction2_Z);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool Collision::sphereSwept(const XMVECTOR& pos1, const XMVECTOR& speed1, const float& r1, const XMVECTOR& pos2, const XMVECTOR& speed2, const float& r2,
//	float &outSecond,	//進行度格納用ポインター
//	XMVECTOR &CollisionPos,	//衝突位置(接点)
//	XMVECTOR *CollisionPosA,	//一つ目が衝突した時の位置
//	XMVECTOR *CollisionPosB	//二つ目が衝突した時の位置
//)
//{
//	XMVECTOR Cstart = pos2 - pos1;				//移動前のabの距離
//	XMVECTOR Aend = pos1 + speed1;		//移動後のaの位置
//	XMVECTOR Bend = pos2 + speed2;		//移動後のbの位置
//	XMVECTOR Cend = Bend - Aend;					//終了時のabの距離
//	XMVECTOR D = Cend - Cstart;					//終了時のabの距離
//	float RadiusAB = r1 + r2;					//半径の合計
//	float RadiusABpow = RadiusAB * RadiusAB;				//半径の合計の二乗
//	float P = D.LengthSq();	//Dの長さ
//
//	//平行移動
//	if (P == 0) {
//		if (XMVECTOR(pos2 - pos1).LengthSq() > RadiusABpow) {
//			return false;
//		}
//		outSecond = 0.0f;	//進行度0からスタート
//		if (CollisionPosA != 0) {
//			*CollisionPosA = pos1;
//		}
//		if (CollisionPosB != 0) {
//			*CollisionPosB = pos2;
//		}
//		//中心位置が同じなので中心で衝突したことにする
//		if (pos2 == pos1) {
//			CollisionPos = pos1;
//			return true;
//		}
//		//Bの方向に向かってAの半径を衝突地点とする
//		CollisionPos = pos1 + Cstart * r1 / RadiusAB/* Vector2<float>{(a.r /RadiusAB)*Cstart.x, (a.r / RadiusAB) *Cstart.y}*/;
//		return true;
//	}
//	//衝突検知
//	//最初の時点で衝突しているかいないか
//	if (XMVECTOR(pos2 - pos1).LengthSq() <= RadiusABpow) {
//		outSecond = 0.0f;
//		CollisionPos = pos1 + Cstart * r1 / RadiusAB;
//		if (CollisionPosA != 0) {
//			*CollisionPosA = pos1;
//		}
//		if (CollisionPosB != 0) {
//			*CollisionPosB = pos2;
//		}
//		return true;
//	}
//	float Q = Cstart.VDot(D);
//	float R = Cstart.LengthSq();
//
//	float judge = Q * Q - P * (R - RadiusAB * RadiusAB);
//	if (judge < 0) {
//		return false;
//	}
//
//	//衝突時間計算
//	float judge_rt = sqrtf(judge);
//	float t_plus = (-Q + judge_rt) / P;
//	float t_minus = (-Q - judge_rt) / P;
//	if (t_minus > t_plus) {	//plusの方が小さければ入れ替え
//		float tmp = t_minus;
//		t_minus = t_plus;
//		t_plus = tmp;
//	}
//
//	//時間外の衝突だった場合を無効にする
//	if (t_minus < 0.0f || t_minus > 1.0f) {
//		return false;
//	}
//
//	//衝突位置計算
//	outSecond = t_minus;
//	XMVECTOR actA = pos1 + speed1 *  t_minus;
//	XMVECTOR actB = pos2 + speed2 *  t_minus;
//	CollisionPos = actA + (actB - actA) * r1 / RadiusAB;
//
//	if (CollisionPosA != 0) {
//		*CollisionPosA = actA;
//	}
//	if (CollisionPosB != 0) {
//		*CollisionPosB = actB;
//	}
//
//	return true;
//}
//
//bool Collision::IsPredictCollisionBall(const XMVECTOR& pos1, const XMVECTOR& speed1, const float& r1, const XMVECTOR& pos2, const XMVECTOR& speed2, const float& r2)
//{
//	// 中心点間の距離
//	distance = pos1 - pos2;
//
//	return distance.Length() < ((r1 + speed1.Length()) + (r2 + speed2.Length()));
//}
//
