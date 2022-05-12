#include "FbxSphereCollider.h"
//#include "Object3d.h"

using namespace DirectX;

void FbxSphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const XMMATRIX& matWorld = fbxobject3d->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}
