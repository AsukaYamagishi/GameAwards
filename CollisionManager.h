#pragma once

#include<forward_list>
#include"KeyboardInput.h"

class BaseCollider;

class CollisionManager
{
public://�ÓI�����o�֐�
	static CollisionManager* GetInstance();

public://�����o�֐�
	///<summary>
	///�R���C�_�[�̒ǉ�
	///</summary>
	///<param name="collider">�R���C�_�[</param>
	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	///<summary>
	///�R���C�_�[�̍폜
	///</summary>
	///<param name="collider">�R���C�_�[</param>
	inline void RemoveCollider(BaseCollider* collider) {
		colliders.remove(collider);
	}

	///<summary>
	///�S�Ă̏Փ˂��`�F�b�N
	///</summary>
	void CheckAllCollision(bool hit[], const bool attackFlag[],const KeyboardInput &input);
	~CollisionManager();

	void Finalize();

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//�R���C�_�[�̃��X�g
	std::forward_list<BaseCollider*>colliders;
};