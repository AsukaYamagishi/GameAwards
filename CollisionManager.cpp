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
	//全ての組み合わせについて総当たりチェック
	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

#pragma region 初期当たり判定（全タイプの判定をして重くなるのでコメントアウット）
			////ともに球
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
			////メッシュと球
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
			// //球とメッシュ
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

			 ////プレイヤーとボスの頭
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



			 //ハンマーとボスの当たり判定
#pragma region ハンマーとボスの当たり判定
			  //プレイヤーとボスの頭
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
			//プレイヤーとボスの体
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
			//プレイヤーとボスの右腕
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
			//プレイヤーとボスの左腕
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
			//プレイヤーとボスの右足
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
			//プレイヤーとボスの左足
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
			//プレイヤーとボスの頭
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
			//プレイヤーとボスの体
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
			//プレイヤーとボスの右腕
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
			//プレイヤーとボスの左腕
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
			//プレイヤーとボスの右足
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
			//プレイヤーとボスの左足
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

#pragma region ボスの球とプレイヤー
			//ボスの球とプレイヤー
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
			//ボスの球とプレイヤー
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
