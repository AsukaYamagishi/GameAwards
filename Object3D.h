#pragma once
#include<DirectXTex.h>
#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include <d3dcompiler.h>


using namespace Microsoft::WRL;
using namespace DirectX;
#pragma comment(lib, "d3dcompiler.lib")


class Object3D
{
#pragma region �\����
public: //�\����
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; //xyz
		XMFLOAT3 normal; //�@���x�N�g��
		XMFLOAT2 uv; //uv
	};
	//�萔�o�b�t�@�f�[�^�\���́i���肽���f�[�^���������炱���ɏ����j
	struct ConstBufferData
	{
		XMFLOAT4 color; //�F(RGBA)
		XMMATRIX mat; //3D�ϊ��s��
	};
#pragma endregion

#pragma region �ϐ�
private: //�萔
	static const int division = 50; //������
	static const float radius; //��ʂ̔��a
	static const float prizmHeight; //���̍���
	static const int planeCount = division * 2 + division * 2; //�ʂ̐�
	static const int vertexCount = planeCount * 3; //���_��
	//static const int spriteSRVCount = 512; //�e�N�X�`���̍ő喇��

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	//�f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	//�V�F�[�_�[���\�[�X�r���[�̃n���h��(GPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//�r���[�s��
	static XMMATRIX matView;
	//�ˉe�s��
	static XMMATRIX matProjection;
	//���_�s��
	static XMFLOAT3 eye;
	//�����_�s��
	static XMFLOAT3 target;
	//������x�N�g��
	static XMFLOAT3 up;
	//���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//�C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	//���_�f�[�^�z��
	static VertexPosNormalUv vertices[vertexCount];
	//���_�C���f�b�N�X�z��
	static unsigned short indices[planeCount * 3];

private: //�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//XYZ���̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//�e�I�u�W�F�N�g
	Object3D* parent = nullptr;
#pragma endregion

#pragma region �֐�
public: //�ÓI�����o�֐�
	//�ÓI������
	static bool StaticInit(ID3D12Device* dev, int window_width, int window_height);
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//�`��㏈��
	static void PostDraw();
	//3D�I�u�W�F�N�g����
	static Object3D* CreateObject();
	//�e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	//���_���W�̎擾
	static const XMFLOAT3& GetEye() { return eye; }
	//���_���W�̐ݒ�
	static void SetEye(XMFLOAT3 eye);
	//�����_���W�̎擾
	static const XMFLOAT3& GetTarget() { return target; }
	//�����_���W�̐ݒ�
	static void SetTarget(XMFLOAT3 target);
	//�x�N�g���ɂ��ړ�
	static void CameraMoveVector(XMFLOAT3 move);

private: //�ÓI�����o�֐�
	//�f�X�N���v�^�q�[�v������
	static bool InitDescHeap();
	//�J����������
	static void InitCamera(int window_width, int window_height);
	//�O���t�B�b�N�p�C�v���C������
	static bool InitGraphicsPipeline();
	//���f���쐬
	static void CreateModel();
	//�r���[�s����X�V
	static void UpdateViewMatrix();

public: //�����o�֐�
	//������
	bool Init();
	//���t���[������
	void Update();
	//�`��
	void Draw();
	//���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }
	//���W�̐ݒ�
	void SetPosition(XMFLOAT3 position) { this->position = position; }
#pragma endregion

};

