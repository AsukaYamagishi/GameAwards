#pragma once
///<summary>
///�����蔻��w���p�[�N���X
///</summary>

#include <DirectXMath.h>
#include "CollisionPrimitive.h"
#include "Vector3.h"

class Collision
{
public:
	///<summary>
    ///���ƕ��ʂ̓����蔻��
    ///</summary>
	///<param name="sphere">��</param>
	///<param name="plane">����</param>
	///<param name="inter">��_(���ʏ�̍ŋߐړ_)</param>
	///<returns>�������Ă��邩�ۂ�</returns>
	static bool IsSphereToPlane(
		const Sphere &sphere,
		const Plane &plane,
		DirectX::XMVECTOR *inter = nullptr
	);

	///<summary>
	///�_�ƎO�p�`�̍ŋߐړ_�����߂�
	///</summary>
	///<param name="point">�_</param>
	///<param name="triangle">�O�p�`</param>
	///<param name="closest">�ŋߐړ_(�o�͗p)</param>
	static void ClossPointToTriangle(
		const DirectX::XMVECTOR &point,
		const Triangle &triangle,
		DirectX::XMVECTOR *closest
	);

	///<summary>
	///���Ɩ@���t���O�p�`�̓����蔻��
	///</summary>
	///<param name="sphere">��</param>
	///<param name="triangle">�O�p�`</param>
	///<param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	///<returns>�������Ă��邩�ۂ�</returns>
	static bool IsSqhereToTriangle(
		const Sphere &sphere,
		const Triangle &triangle,
		DirectX::XMVECTOR *inter = nullptr
	);

	//���C�ƕ���
	static bool IsRayToPlane(
		const Ray &ray,
		const Plane &plane,
		float *distance = nullptr,
		DirectX::XMVECTOR *inter = nullptr
	);

	//���C�Ɩ@���t���O�p
	static bool IsRayToTriangle(
		const Ray &ray,
		const Triangle &triangle,
		float *distance = nullptr,
		DirectX::XMVECTOR *inter = nullptr
	);

	//���C�Ƌ�
	static bool IsRayToSqhere(
		const Ray &ray,
		const Sphere &sphere,
		float *distance = nullptr,
		DirectX::XMVECTOR *inter = nullptr
	);

    // ���̓��m�̓����蔻��
	static bool IsBallToBallCollision(const Sphere& a,const Sphere& b);
//
//	// �����̓��m�̓����蔻��
//	bool IsBoxToBoxCollision(const XMVECTOR& pos1, const XMVECTOR& rotation1, const XMVECTOR& scale1, const XMVECTOR& pos2, const XMVECTOR& rotation2, const XMVECTOR& scale2);
//
//
//
//	// ���̃X�C�[�v�i�ړ����鋅���m�̔���j
//    // �������Ă�����Փ˂������Ԃ�0~1�̊ԂŕԂ��A�������Ă��Ȃ�������-1��Ԃ�
//	bool sphereSwept(const XMVECTOR& pos1, const XMVECTOR& speed1, const float& r1, const XMVECTOR& pos2, const XMVECTOR& speed2, const float& r2,
//	float &outSecond,	//�i�s�x�i�[�p�|�C���^�[
//	XMVECTOR &CollisionPos,	//�Փˈʒu(�ړ_)
//	XMVECTOR *CollisionPosA = 0,	//��ڂ��Փ˂������̈ʒu
//	XMVECTOR *CollisionPosB = 0	//��ڂ��Փ˂������̈ʒu
//);
//
//	// �Փ˗\��
//    // ���̏ꍇ�́AXMVECTOR(����, ����, ���s).Length() / 2������Ύg���܂�
//	bool IsPredictCollisionBall(const XMVECTOR& pos1, const XMVECTOR& speed1, const float& r1, const XMVECTOR& pos2, const XMVECTOR& speed2, const float& r2);

};
