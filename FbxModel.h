#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

//�m�[�h�\����
struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	DirectX::XMMATRIX transform;
	//�O���[�o���ό`�s��
	DirectX::XMMATRIX globalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};


class FbxModel
{
public:
	//�t�����h�N���X
	friend class FbxInput;

private: //�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	using string = std::string;
	template <class T> using vector = std::vector<T>;


public: //�萔
//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

public: //�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; //xyz���W
		DirectX::XMFLOAT3 normal; //�@���x�N�g��
		DirectX::XMFLOAT2 uv; //uv���W
		UINT boneIndex[MAX_BONE_INDICES]; //�{�[�� �ԍ�
		float boneWeight[MAX_BONE_INDICES]; //�{�[�� �d��
	};

	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};


public: //�����o�֐�
	//�f�X�g���N�^
	~FbxModel();
	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* dev);
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);


public: //�Q�b�^�[�E�Z�b�^�[
	//���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }
	//�{�[�����擾
	std::vector<Bone>& GetBones() { return bones; }
	//FBX�V�[�����擾
	FbxScene* GetFbxScene() { return fbxScene; }
	//���_���̎擾
	std::vector<VertexPosNormalUvSkin>* GetVertices() { return &vertices; }
	//�C���f�b�N�X�̎擾
	std::vector<unsigned short>* GetIndces() { return &indices; }

	//�����蔻�莞�Ɏg���悤�ł������ς������
	std::vector<VertexPosNormalUvSkin>& GetColVertices() { return vertices; }
	std::vector<unsigned short>& GetColIndces() { return indices; }

private: //�����o�ϐ�
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	//�A���r�G���g�W��
	DirectX::XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�q���[�Y�W��
	DirectX::XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	DirectX::TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	DirectX::ScratchImage scratchImg = {};
	//�{�[���z��
	std::vector<Bone> bones;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

};

