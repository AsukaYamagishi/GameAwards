#include "Collision.h"
#include <DirectXMath.h>

using namespace DirectX;

static XMVECTOR distance{}; //��_�Ԃ̋���(Collision namespace������)

bool Collision::IsSphereToPlane(const Sphere &sphere, const Plane &plane, XMVECTOR *inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center,plane.normal);
	//���ʂ̌��_���������Y���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a�����傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) {
		return false;
	}
	//�^����_���v�Z
	if (inter) {
		//���ʏ�̍ŋߐړ_���^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}

void Collision::ClossPointToTriangle(const XMVECTOR &point, const Triangle &triangle, XMVECTOR *closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1,p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2,p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return ;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1,p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2,p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1,p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2,p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
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
	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClossPointToTriangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	//�����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	v = XMVector3Dot(v, v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius) {
		return false;
	}
	//�^����_���v�Z
	if (inter) {
		//�O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	return true;
}

bool Collision::IsRayToPlane(const Ray &ray, const Plane &plane, float *distance, XMVECTOR *inter)
{
	const float epsilon = 1.0e-5f;	//�덷�z���p
	float d1 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) {
		return false;
	}
	//�ʖ@���Ɨ�̎n�_���W�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//�n�_�ƕ��ʂ̋����i���ʂ̖@�������j
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋����i���C�����j
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂œ�����Ȃ�
	if (t < 0) {
		return false;
	}
	//��������������
	if (distance) {
		*distance = t;
	}
	//��_���v�Z
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
	//���C�ƕ��ʂ��������Ă��Ȃ���Γ������Ă��Ȃ�
	if (!IsRayToPlane(ray, plane, distance, &interPlane)) {
		return false;
	}
	
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;
	XMVECTOR m;
	
	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ�
	if (XMVector3Dot(m,triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	//��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	//��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	if (inter) {
		*inter = interPlane;
	}
	return true;

}

bool Collision::IsRayToSqhere(const Ray &ray, const Sphere &sphere, float *distance, XMVECTOR *inter) {
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;

	if (c > 0.0f && b > 0.0f) {
		return false;
	}
	float discr = b * b - c;
	//���̔��莮�͗Ⴊ�����O��Ă��邱�Ƃƈ�v
	if (discr < 0.0f) {
		return false;
	}

	//���C�Ƌ��͌������Ă���B
	float t = -b - sqrtf(discr);
	//t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂ŁAt��0��
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
		// ���S�_�Ԃ̋���
		Vector3 distance = a.center - b.center;

		return distance.Length() < (a.radius + b.radius);
	}
//
//bool Collision::IsBoxToBoxCollision(const XMVECTOR& pos1, const XMVECTOR& rotation1, const XMVECTOR& scale1, const XMVECTOR& pos2, const XMVECTOR& rotation2, const XMVECTOR& scale2)
//{
//	using namespace DirectX;
//
//	// ��]�s��
//	static XMMATRIX matRot1{}, matRot2{};
//	// ���S����ʂ܂ł̒���
//	static XMVECTOR length1{}, length2{};
//	// �������x�N�g��
//	static XMVECTOR direction1_XN{}, direction1_YN{}, direction1_ZN{}, direction2_XN{}, direction2_YN{}, direction2_ZN{};
//	// �����x�N�g��
//	static XMVECTOR direction1_X{}, direction1_Y{}, direction1_Z{}, direction2_X{}, direction2_Y{}, direction2_Z{};
//
//	/*OBB�̓����蔻��̉�����*/
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
//	/*OBB�̓����蔻��*/
//
//	distance = pos1 - pos2;
//
//	static float r1{}, r2{}; //�����e����
//	static float l{}; //���ԓ_�Ԃ̋���
//
//	// �������x�N�g���Fdirection1_XN
//	r1 = direction1_X.Length();
//	r2 = fabsf(direction1_XN.VDot(direction2_X)) + fabsf(direction1_XN.VDot(direction2_Y)) + fabsf(direction1_XN.VDot(direction2_Z));
//	l = fabsf(direction1_XN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���Fdirection1_YN
//	r1 = direction1_Y.Length();
//	r2 = fabsf(direction1_YN.VDot(direction2_X)) + fabsf(direction1_YN.VDot(direction2_Y)) + fabsf(direction1_YN.VDot(direction2_Z));
//	l = fabsf(direction1_YN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���Fdirection1_ZN
//	r1 = direction1_Z.Length();
//	r2 = fabsf(direction1_ZN.VDot(direction2_X)) + fabsf(direction1_ZN.VDot(direction2_Y)) + fabsf(direction1_ZN.VDot(direction2_Z));
//	l = fabsf(direction1_ZN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���Fdirection2_XN
//	r1 = direction2_X.Length();
//	r2 = fabsf(direction2_XN.VDot(direction1_X)) + fabsf(direction2_XN.VDot(direction1_Y)) + fabsf(direction2_XN.VDot(direction1_Z));
//	l = fabsf(direction2_XN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���Fdirection2_YN
//	r1 = direction2_Y.Length();
//	r2 = fabsf(direction2_YN.VDot(direction1_X)) + fabsf(direction2_YN.VDot(direction1_Y)) + fabsf(direction2_YN.VDot(direction1_Z));
//	l = fabsf(direction2_YN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���Fdirection2_ZN
//	r1 = direction2_Z.Length();
//	r2 = fabsf(direction2_ZN.VDot(direction1_X)) + fabsf(direction2_ZN.VDot(direction1_Y)) + fabsf(direction2_ZN.VDot(direction1_Z));
//	l = fabsf(direction2_ZN.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	static XMVECTOR crossDirection{}; //�O�ϕ�����
//
//	// �������x�N�g���FCross(direction1_X, direction2_X)
//	crossDirection = direction1_X.VCross(direction2_X);
//	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_X, direction2_Y)
//	crossDirection = direction1_X.VCross(direction2_Y);
//	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_X, direction2_Z)
//	crossDirection = direction1_X.VCross(direction2_Z);
//	r1 = fabsf(crossDirection.VDot(direction1_Y)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_Y, direction2_X)
//	crossDirection = direction1_Y.VCross(direction2_X);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_Y, direction2_Y)
//	crossDirection = direction1_Y.VCross(direction2_Y);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_Y, direction2_Z)
//	crossDirection = direction1_Y.VCross(direction2_Z);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Z));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Y));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_Z, direction2_X)
//	crossDirection = direction1_Z.VCross(direction2_X);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
//	r2 = fabsf(crossDirection.VDot(direction2_Y)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_Z, direction2_Y)
//	crossDirection = direction1_Z.VCross(direction2_Y);
//	r1 = fabsf(crossDirection.VDot(direction1_X)) + fabsf(crossDirection.VDot(direction1_Y));
//	r2 = fabsf(crossDirection.VDot(direction2_X)) + fabsf(crossDirection.VDot(direction2_Z));
//	l = fabsf(crossDirection.VDot(distance));
//	if ((r1 + r2) < l)
//	{
//		return false;
//	}
//
//	// �������x�N�g���FCross(direction1_Z, direction2_Z)
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
//	float &outSecond,	//�i�s�x�i�[�p�|�C���^�[
//	XMVECTOR &CollisionPos,	//�Փˈʒu(�ړ_)
//	XMVECTOR *CollisionPosA,	//��ڂ��Փ˂������̈ʒu
//	XMVECTOR *CollisionPosB	//��ڂ��Փ˂������̈ʒu
//)
//{
//	XMVECTOR Cstart = pos2 - pos1;				//�ړ��O��ab�̋���
//	XMVECTOR Aend = pos1 + speed1;		//�ړ����a�̈ʒu
//	XMVECTOR Bend = pos2 + speed2;		//�ړ����b�̈ʒu
//	XMVECTOR Cend = Bend - Aend;					//�I������ab�̋���
//	XMVECTOR D = Cend - Cstart;					//�I������ab�̋���
//	float RadiusAB = r1 + r2;					//���a�̍��v
//	float RadiusABpow = RadiusAB * RadiusAB;				//���a�̍��v�̓��
//	float P = D.LengthSq();	//D�̒���
//
//	//���s�ړ�
//	if (P == 0) {
//		if (XMVECTOR(pos2 - pos1).LengthSq() > RadiusABpow) {
//			return false;
//		}
//		outSecond = 0.0f;	//�i�s�x0����X�^�[�g
//		if (CollisionPosA != 0) {
//			*CollisionPosA = pos1;
//		}
//		if (CollisionPosB != 0) {
//			*CollisionPosB = pos2;
//		}
//		//���S�ʒu�������Ȃ̂Œ��S�ŏՓ˂������Ƃɂ���
//		if (pos2 == pos1) {
//			CollisionPos = pos1;
//			return true;
//		}
//		//B�̕����Ɍ�������A�̔��a���Փ˒n�_�Ƃ���
//		CollisionPos = pos1 + Cstart * r1 / RadiusAB/* Vector2<float>{(a.r /RadiusAB)*Cstart.x, (a.r / RadiusAB) *Cstart.y}*/;
//		return true;
//	}
//	//�Փˌ��m
//	//�ŏ��̎��_�ŏՓ˂��Ă��邩���Ȃ���
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
//	//�Փˎ��Ԍv�Z
//	float judge_rt = sqrtf(judge);
//	float t_plus = (-Q + judge_rt) / P;
//	float t_minus = (-Q - judge_rt) / P;
//	if (t_minus > t_plus) {	//plus�̕�����������Γ���ւ�
//		float tmp = t_minus;
//		t_minus = t_plus;
//		t_plus = tmp;
//	}
//
//	//���ԊO�̏Փ˂������ꍇ�𖳌��ɂ���
//	if (t_minus < 0.0f || t_minus > 1.0f) {
//		return false;
//	}
//
//	//�Փˈʒu�v�Z
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
//	// ���S�_�Ԃ̋���
//	distance = pos1 - pos2;
//
//	return distance.Length() < ((r1 + speed1.Length()) + (r2 + speed2.Length()));
//}
//
