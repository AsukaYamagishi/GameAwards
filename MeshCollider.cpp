#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;

void MeshCollider::ConstrucTriangles(ModelInput* model)
{
	//�O�p�`���X�g���N���A
	triangles.clear();
	//���f���̎����b�V�����X�g���擾
	//const std::vector<Mesh*>& meshes = model->GetMeshes();
	//���݂̃��b�V���̊J�n�O�p�`�ԍ������Ă����ϐ��i0�ŏ������j
	int start = 0;
	//�S���b�V���ɂ��ď��ɏ�������
	//std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	//for (; it != meshes.cend(); ++it)
	{
		//Mesh* mesh = *it;
		const std::vector<ModelInput::VertexPosNormalUv>& vertices = model->GetVertices();
		const std::vector<unsigned short>& indices = model->GetIndices();
		//�C���f�b�N�X�́A�O�p�`�̐��~3�ł���̂ŁA
		//�������烁�b�V�����̎O�p�`�̐����t�Z����
		size_t triangleNum = indices.size() / 3;
		//���݂̃��b�V���̎O�p�`�̐������A�O�p�`���X�g�ɃX�y�[�X��ǉ�����
		triangles.resize(triangles.size() + triangleNum);
		//�S�O�p�`�ɂ��ď��ɏ�������
		for (int i = 0; i < triangleNum; i++)
		{
			//������v�Z����O�p�`�̎Q��
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];
			//�O�p�`��3���_�̍��W����
			tri.p0 = { vertices[idx0].pos.x,vertices[idx0].pos.y,vertices[idx0].pos.z,1 };
			tri.p1 = { vertices[idx1].pos.x,vertices[idx1].pos.y,vertices[idx1].pos.z,1 };
			tri.p2 = { vertices[idx2].pos.x,vertices[idx2].pos.y,vertices[idx2].pos.z,1 };
			//3���_����@�����v�Z
			tri.ComputeNormal();
		}
		//���̃��b�V���́A���܂ł̎O�p�`�ԍ��̎�����g��
		start += (int)triangleNum;
	}
}

void MeshCollider::Update()
{
	//���[���h�s��̋t�s����v�Z
	invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	//�I�u�W�F�Nt�̃��[�J�����W�n�ł̋��𓾂�
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	//���[�J�����W�n�ł̌������`�F�b�N
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;
		//���ƎO�p�`�̓����蔻��
		if (Collision::IsSqhereToTriangle(localSphere, triangle, inter)) {
			if (inter) {
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				//���[���h���W�n�ł̌����𓾂�
				*inter = XMVector3Transform(*inter, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter)
{
	return false;
}
