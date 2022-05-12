#pragma once

#include<forward_list>
#include"KeyboardInput.h"

class BaseCollider;

class CollisionManager
{
public://静的メンバ関数
	static CollisionManager* GetInstance();

public://メンバ関数
	///<summary>
	///コライダーの追加
	///</summary>
	///<param name="collider">コライダー</param>
	inline void AddCollider(BaseCollider* collider) {
		colliders.push_front(collider);
	}

	///<summary>
	///コライダーの削除
	///</summary>
	///<param name="collider">コライダー</param>
	inline void RemoveCollider(BaseCollider* collider) {
		colliders.remove(collider);
	}

	///<summary>
	///全ての衝突をチェック
	///</summary>
	void CheckAllCollision(bool hit[], const bool attackFlag[],const KeyboardInput &input);
	~CollisionManager();

	void Finalize();

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//コライダーのリスト
	std::forward_list<BaseCollider*>colliders;
};