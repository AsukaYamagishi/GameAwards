#pragma once
#include "FbxBaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

//���b�V���Փ˔���I�u�W�F�N�g
class FbxMeshCollider :
	public FbxBaseCollider
{
public:
	FbxMeshCollider()
	{
		//���b�V���`����Z�b�g
		shapeType = COLLISIONSHAPE_MESH;
	}

	//�O�p�`�̔z����\�z����
	void ConstrucTriangles(FbxModel* model);

	//�X�V
	void Update()override;

	//���Ƃ̓����蔻��
	bool CheckCollisionSphere(const Sphere& sphere,
		DirectX::XMVECTOR* inter = nullptr);

	//���C�Ƃ̓����蔻��
	bool CheckCollisionRay(const Ray& ray, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

private:
	std::vector<Triangle>triangles;
	//���[���h�s��̋t�s��
	DirectX::XMMATRIX invMatWorld;
};

