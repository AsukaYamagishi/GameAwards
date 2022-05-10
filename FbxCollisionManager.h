#pragma once

#include<forward_list>

class FbxBaseCollider;

class FbxCollisionManager
{
public://静的メンバ関数
	static FbxCollisionManager* GetInstance();

public://メンバ関数
	///<summary>
	///コライダーの追加
	///</summary>
	///<param name="collider">コライダー</param>
	inline void AddCollider(FbxBaseCollider* collider) {
		colliders.push_front(collider);
	}

	///<summary>
	///コライダーの削除
	///</summary>
	///<param name="collider">コライダー</param>
	inline void RemoveCollider(FbxBaseCollider* collider) {
		colliders.remove(collider);
	}

	///<summary>
	///全ての衝突をチェック
	///</summary>
	void CheckAllCollision(bool hit[]);
	~FbxCollisionManager();

	void Finalize();

private:
	FbxCollisionManager() = default;
	FbxCollisionManager(const FbxCollisionManager&) = delete;
	FbxCollisionManager& operator=(const FbxCollisionManager&) = delete;
	//コライダーのリスト
	std::forward_list<FbxBaseCollider*>colliders;
};