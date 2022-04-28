#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
#include <DirectXTex.h>
using namespace DirectX;

#include "Vector3.h"
//ComPtr
#include <wrl.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "ModelInput.h"
#include "Camera.h"
#include"CollisionInfo.h"

class BaseCollider;

using namespace std;

class ModelDraw
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;	//Microsoft::WRL::���ȗ�
public:	 //�T�u�N���X
	//�p�C�v���C���\����
	struct PipelineSet {
		ComPtr<ID3D12PipelineState> pipelinestate;
		ComPtr<ID3D12RootSignature> rootsignature;
	};

	//�I�u�W�F�N�g�̋��ʃf�[�^
	static const int spriteSRVCount = 512;
	struct ObjectCommon {
		ID3D12Device *dev;			//�f�o�C�X
		ID3D12GraphicsCommandList *cmdList;		//�R�}���h���X�g

		PipelineSet pipelineSet;				//�p�C�v���C���Z�b�g
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferDataB0 {
		XMMATRIX mat;	//3D�ϊ��s��
		XMMATRIX matBillboard;	// �r���{�[�h�s��
	};
	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1 {
		XMFLOAT3 ambient;	//�A���r�G�C�g�W��
		float pad1;			//�p�f�B���O
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		XMFLOAT3 specular;	//�X�y�L�����[�W��
		float alpha;		//�p�f�B���O
	};

	enum BLENDMODE {
		NORMAL,
		ADD,
		SUB,
	};

public:	//�ÓI�����o�֐�
	//�ÓI������
	static bool StaticInitialize(ID3D12Device *dev, int window_width, int window_height);
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList *cmdList);
	//�`��㏈��
	static void PostDraw();
	//�I�u�W�F�N�g����������
	static ModelDraw *Create(bool isBillboard = false);

	//�u�����h���[�h�ύX
	static void BlendMode(BLENDMODE blendM);

private:	//�ÓI�����o�֐�
	//3D�I�u�W�F�N�g�p�p�C�v���C��
	static bool InitializeGraphicsPipeline();

public:	//�ÓI�����o�ϐ�
	static ObjectCommon objectCommon;				//�悭�g����
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	//Y������r���{�[�h�s��
	static XMMATRIX matBillboardY;

public:		//�����o�֐�
	//������
	bool Initialize();
	//���f���Z�b�g
	void SetModel(ModelInput *model);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();

	void SetPos(Vector3 nextPos) { position = nextPos; }
	void SetScale(Vector3 nextScale) { scale = nextScale; }
	void SetRotation(Vector3 nextRotation) { rotation = nextRotation; }
	void SetParent(ModelDraw* nextParent) { parent = nextParent; }
	void SetBillboard(bool isBillboard) { ModelDraw::isBillboard = isBillboard; }
	Vector3 GetPos() { return position; }
	Vector3 GetScale() { return scale; }
	Vector3 GetRotation() { return rotation; }
	ModelDraw* GetParent() { return parent; }
	
/// <summary>
/// �R���C�_�[�̃Z�b�g
/// </summary>
/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);
/// <summary>
/// �Փˎ��R�[���o�b�N�֐�
/// </summary>
/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info);

/// <summary>
/// ���[���h�s��̎擾
/// </summary>
/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	//���f���C���v�b�g���擾
	inline const ModelInput& GetMeshes() { return *objectModel; }

	//���f���C���v�b�g���擾
	inline  ModelInput *GetModelInput() { return objectModel; }


	// �R���C�_�[
	BaseCollider* collider = nullptr;
private:	//�����o�ϐ�
	XMFLOAT4 color = { 1,1,1,1 };	//�F
	Vector3 scale = { 1,1,1 };		//�X�P�[��	
	Vector3 rotation = { 0,0,0 };	//��]
	Vector3 position = { 0,0,0 };	//���W
	bool isBillboard;

	ModelInput *objectModel = nullptr;					//���f���|�C���^�[
	ComPtr<ID3D12Resource> constBuffB0;		//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1;		//�萔�o�b�t�@
	XMMATRIX matWorld;						//���[���h�ϊ��s��
	UINT texNumber = 0;						//�e�N�X�`���ԍ�
	ModelDraw *parent = nullptr;			//�e�I�u�W�F�N�g�ւ̃|�C���^
};