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

void CollisionManager::CheckAllCollision(bool hit[], const bool attackFlag[], const KeyboardInput& input)
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

			if (attackFlag[PlayerAttack] || input.PressKey(DIK_R)) {
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
						return;
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
			}
			if (attackFlag[PlayerAttack]) {
#pragma region �v���C���[�������ƃ{�X�̓����蔻��
				//�v���C���[�ƃ{�X�̓�
				if (colA->tag == TagHead &&
					colB->tag == TagPlayerBullet)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[WwaponToHead] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̑�
				if (colA->tag == TagBody &&
					colB->tag == TagPlayerBullet)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[WwaponToBody] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̉E�r
				if (colA->tag == TagRightArm &&
					colB->tag == TagPlayerBullet)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[WwaponToRightArm] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̍��r
				if (colA->tag == TagLeftArm &&
					colB->tag == TagPlayerBullet)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[WwaponToLeftArm] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̉E��
				if (colA->tag == TagRightLeg &&
					colB->tag == TagPlayerBullet)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[WwaponToRightLeg] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̍���
				if (colA->tag == TagLeftLeg &&
					colB->tag == TagPlayerBullet)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[WwaponToLeftLeg] = 1;
					}
				}

				//--------------------------------------------------------------------------------//
				//�v���C���[�ƃ{�X�̓�
				if (colA->tag == TagPlayerBullet &&
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
				if (colA->tag == TagPlayerBullet &&
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
				if (colA->tag == TagPlayerBullet &&
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
				if (colA->tag == TagPlayerBullet &&
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
				if (colA->tag == TagPlayerBullet &&
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
				if (colA->tag == TagPlayerBullet &&
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
			}

			/*if (attackFlag[BossPress]) {*/
#pragma region �{�X�̃v���X�ƃv���C���[�̓����蔻��
				//�{�X�̍U���ƃv���C���[�{�f�B
				if (colA->tag == TagHead &&
					colB->tag == TagPlayer)
				{					
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToHead] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̉E�r
				if (colA->tag == TagRightArm &&
					colB->tag == TagPlayer)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToRightArm] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̍��r
				if (colA->tag == TagLeftArm &&
					colB->tag == TagPlayer)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToLeftArm] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̉E��
				if (colA->tag == TagRightLeg &&
					colB->tag == TagPlayer)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToRightLeg] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̍���
				if (colA->tag == TagLeftLeg &&
					colB->tag == TagPlayer)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
					Sphere* sphere = dynamic_cast<Sphere*>(colB);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToLeftLeg] = 1;
					}
				}

				//--------------------------------------------------------------------------------//
				//�v���C���[�ƃ{�X�̑�
				if (colA->tag == TagPlayer &&
					colB->tag == TagHead)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToHead] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̉E�r
				if (colA->tag == TagPlayer &&
					colB->tag == TagRightArm)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToRightArm] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̍��r
				if (colA->tag == TagPlayer &&
					colB->tag == TagLeftArm)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToLeftArm] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̉E��
				if (colA->tag == TagPlayer &&
					colB->tag == TagRightLeg)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToRightLeg] = 1;
					}
				}
				//�v���C���[�ƃ{�X�̍���
				if (colA->tag == TagPlayer &&
					colB->tag == TagLeftLeg)
				{
					MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
					Sphere* sphere = dynamic_cast<Sphere*>(colA);
					DirectX::XMVECTOR inter;
					if (meshCollider->CheckCollisionSphere(*sphere, &inter))
					{
						hit[PlayerToLeftLeg] = 1;
					}
				}
#pragma endregion
			//}

			//�{�X�̓ːi�ƃv���C���[�̓����蔻��
				if (attackFlag[BossRush])
				{
					//�{�X�̍U���ƃv���C���[
					if (colA->tag == TagHead &&
						colB->tag == TagPlayer)
					{
						MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
						Sphere* sphere = dynamic_cast<Sphere*>(colB);
						DirectX::XMVECTOR inter;
						if (meshCollider->CheckCollisionSphere(*sphere, &inter))
						{
							hit[BossAttackToPlayer] = 1;
						}
					}
					//�{�X�̍U���ƃv���C���[
					if (colA->tag == TagBody &&
						colB->tag == TagPlayer)
					{
						MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
						Sphere* sphere = dynamic_cast<Sphere*>(colB);
						DirectX::XMVECTOR inter;
						if (meshCollider->CheckCollisionSphere(*sphere, &inter))
						{
							hit[BossAttackToPlayer] = 1;
						}
					}
				}

			if (attackFlag[BossBeam]) {
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
