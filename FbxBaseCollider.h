#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include"FbxDraw.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class FbxBaseCollider
{
public:
	FbxBaseCollider() = default;
	virtual ~FbxBaseCollider() = default;

	inline void SetObject(FbxDraw* object) {
		this->fbxobject3d = object;
	}

	inline FbxDraw* GetObject3d() {
		return fbxobject3d;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	inline CollisionShapeType GetShapeType() { return shapeType; }

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	inline void OnCollision(const CollisionInfo& info) {
		//object3d->OnCollision(info);
	}

	int tag = 0;	//�R���W�����}�l�[�W���[�ŁA�`��^�C�v�Ƃ̔�r�ȊO�̔�r�p�̂��
protected:
	FbxDraw* fbxobject3d = nullptr;
	// �`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
};

