#include "FbxSphereCollider.h"
//#include "Object3d.h"

using namespace DirectX;

void FbxSphereCollider::Update()
{
	// ���[���h�s�񂩂���W�𒊏o
	const XMMATRIX& matWorld = fbxobject3d->GetMatWorld();

	// ���̃����o�ϐ����X�V
	Sphere::center = matWorld.r[3] + offset;
	Sphere::radius = radius;
}
