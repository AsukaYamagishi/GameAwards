#pragma once
#include "FbxBaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

//メッシュ衝突判定オブジェクト
class FbxMeshCollider :
	public FbxBaseCollider
{
public:
	FbxMeshCollider()
	{
		//メッシュ形状をセット
		shapeType = COLLISIONSHAPE_MESH;
	}

	//三角形の配列を構築する
	void ConstrucTriangles(FbxModel* model);

	//更新
	void Update()override;

	//球との当たり判定
	bool CheckCollisionSphere(const Sphere& sphere,
		DirectX::XMVECTOR* inter = nullptr);

	//レイとの当たり判定
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

private:
	std::vector<Triangle>triangles;
	//ワールド行列の逆行列
	DirectX::XMMATRIX invMatWorld;
};

