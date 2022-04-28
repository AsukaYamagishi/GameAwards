#include "CollisionManager.h"
#include "BaseCollider.h"
#include"Collision.h"
#include"MeshCollider.h"

#include <vector>
#include <list>
#include <string>
#include <unordered_map>

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return &instance;
}

void CollisionManager::CheckAllCollision(bool hit[])
{
	std::forward_list<BaseCollider*>::iterator itA;
	std::forward_list<BaseCollider*>::iterator itB;

	int i = 0;
	//�S�Ă̑g�ݍ��킹�ɂ��đ�������`�F�b�N
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

#pragma region ���������蔻��i�S�^�C�v�̔�������ďd���Ȃ�̂ŃR�����g�A�E�b�g�j
			////�Ƃ��ɋ�
			//if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
			//	colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			//{
			//	Sphere* SphereA = dynamic_cast<Sphere*>(colA);
			//	Sphere* SphereB = dynamic_cast<Sphere*>(colB);
			//	DirectX::XMVECTOR inter;
			//	//if (Collision::CheckSphere2plane(*SphereA, *SphereB, &inter))
			//	{
			//		colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			//		colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
			//	}
			//}
			////���b�V���Ƌ�
			// if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
			//	colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			//{
			//	MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
			//	Sphere* sphere = dynamic_cast<Sphere*>(colB);
			//	DirectX::XMVECTOR inter;
			//	if (meshCollider->CheckCollisionSphere(*sphere, &inter))
			//	{
			//		//hit[i] = 1;
			//		colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			//		colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
			//	}
			//}
			// //���ƃ��b�V��
			// if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
			//	colB->GetShapeType() == COLLISIONSHAPE_MESH)
			//{
			//	MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
			//	Sphere* sphere = dynamic_cast<Sphere*>(colA);
			//	DirectX::XMVECTOR inter;
			//	if (meshCollider->CheckCollisionSphere(*sphere, &inter))
			//	{
			//		colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
			//		colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
			//	}
			//}

			 ////�v���C���[�ƃ{�X�̓�
			 //if (colA->tag == TagHead &&
				// colB->tag == TagPlayer)
			 //{
				// MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				// Sphere* sphere = dynamic_cast<Sphere*>(colB);
				// DirectX::XMVECTOR inter;
				// if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				// {
				//	// hit[7] = 1;
				// }
			 //}
#pragma endregion



			 //�n���}�[�ƃ{�X�̓����蔻��
#pragma region �n���}�[�ƃ{�X�̓����蔻��
			  //�v���C���[�ƃ{�X�̓�
			if (colA->tag == TagHead &&
				colB->tag == TagWeapon)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToHead] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̑�
			if (colA->tag == TagBody &&
				colB->tag == TagWeapon)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToBody] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̉E�r
			if (colA->tag == TagRightArm &&
				colB->tag == TagWeapon)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToRightArm] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̍��r
			if (colA->tag == TagLeftArm &&
				colB->tag == TagWeapon)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToLeftArm] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̉E��
			if (colA->tag == TagRightLeg &&
				colB->tag == TagWeapon)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToRightLeg] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̍���
			if (colA->tag == TagLeftLeg &&
				colB->tag == TagWeapon)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToLeftLeg] = 1;
				}
			}

			//--------------------------------------------------------------------------------//
			//�v���C���[�ƃ{�X�̓�
			if (colA->tag == TagWeapon &&
				colB->tag == TagHead)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToHead] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̑�
			if (colA->tag == TagWeapon &&
				colB->tag == TagBody)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToBody] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̉E�r
			if (colA->tag == TagWeapon &&
				colB->tag == TagRightArm)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToRightArm] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̍��r
			if (colA->tag == TagWeapon &&
				colB->tag == TagLeftArm)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToLeftArm] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̉E��
			if (colA->tag == TagWeapon &&
				colB->tag == TagRightLeg)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToRightLeg] = 1;
				}
			}
			//�v���C���[�ƃ{�X�̍���
			if (colA->tag == TagWeapon &&
				colB->tag == TagLeftLeg)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				DirectX::XMVECTOR inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					hit[WwaponToLeftLeg] = 1;
				}
			}
#pragma endregion

#pragma region �{�X�̋��ƃv���C���[
			//�{�X�̋��ƃv���C���[
			if (colA->tag == TagBullet &&
				colB->tag == TagWeapon)
			{
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::IsBallToBallCollision(*SphereA, *SphereB))
				{
					hit[BulletToPlayer] = 1;
				}
			}
			//�{�X�̋��ƃv���C���[
			if (colA->tag == TagWeapon &&
				colB->tag == TagBullet)
			{
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				DirectX::XMVECTOR inter;
				if (Collision::IsBallToBallCollision(*SphereA, *SphereB))
				{
					hit[BulletToPlayer] = 1;
				}
			}
#pragma endregion

		}
		i++;
	}
	int j = 0;
}

CollisionManager::~CollisionManager()
{
	//delete &colliders;
}

void CollisionManager::Finalize()
{
	//erase(v, LessThan10);
	/*std::forward_list<BaseCollider*>::iterator itA=colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{

	}*/
	colliders.clear();
	//delete& colliders;
}
