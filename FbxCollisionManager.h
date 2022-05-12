#pragma once

#include<forward_list>

class FbxBaseCollider;

class FbxCollisionManager
{
public://�ÓI�����o�֐�
	static FbxCollisionManager* GetInstance();

public://�����o�֐�
	///<summary>
	///�R���C�_�[�̒ǉ�
	///</summary>
	///<param name="collider">�R���C�_�[</param>
	inline void AddCollider(FbxBaseCollider* collider) {
		colliders.push_front(collider);
	}

	///<summary>
	///�R���C�_�[�̍폜
	///</summary>
	///<param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(FbxBaseCollider* collider) {
		colliders.remove(collider);
	}

	///<summary>
	///�S�Ă̏Փ˂��`�F�b�N
	///</summary>
	void CheckAllCollision(bool hit[]);
	~FbxCollisionManager();

	void Finalize();

private:
	FbxCollisionManager() = default;
	FbxCollisionManager(const FbxCollisionManager&) = delete;
	FbxCollisionManager& operator=(const FbxCollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<FbxBaseCollider*>colliders;
};