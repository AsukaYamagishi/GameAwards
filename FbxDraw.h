#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

#include "FbxModel.h"
#include "FbxInput.h"
#include "Camera.h"

class FbxBaseCollider;

class FbxDraw
{
protected: //�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public: //�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public: //�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: //�����o�֐�
	//������
	void Init();

	//���t���[������
	void Update();

	//���f���̃Z�b�g
	void SetModel(FbxModel* model) { this->model = model; }

	//�A�j���[�V�����J�n
	void PlayAnimation(bool isLoop = false);

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);


	//�Q�b�^�[�E�Z�b�^�[
	void SetScale(Vector3 nextScale) { scale = nextScale; }
	void SetPosition(Vector3 nextPos) { position = nextPos; }
	void SetRotation(Vector3 nextRotate) { rotation = nextRotate; }

	void SetCollider(FbxBaseCollider* collider);
	//���[���h�s����擾����֐�
	const XMMATRIX& GetMatWorld() { return matWorld; }
	FbxModel* GetModel() { return model; }

public: //�ÓI�����o�֐�
	//�Z�b�^�[
	static void SetDevice(ID3D12Device* dev) { FbxDraw::dev = dev; }
	static void SetCamera(Camera* camera) { FbxDraw::camera = camera; }

	//�O���t�B�b�N�X�p�C�v���C���̐���
	static void CreateGraphicsPipeline();

	
protected: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�J����
	static Camera* camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//xyz������̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	FbxModel* model = nullptr;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
	//�A�j���[�V�������[�v�Đ�
	bool isLoop = false;

public:
	//�R���C�_�[
	FbxBaseCollider* collider = nullptr;
};

