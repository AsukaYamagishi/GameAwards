#pragma once
#include <d3dx12.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <wrl.h>

using namespace std;
using namespace DirectX;

class ModelInput
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;	//Microsoft::WRL::���ȗ�
public:		//�T�u�N���X

	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	//�}�e���A��
	struct Material {
		string name;		//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		string textureFilename;	//�e�N�X�`���t�@�C����

		// �R���X�g���N�^
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

public:		//�ÓI�����o�֐�
	static bool StaticInitialize(ID3D12Device *dev);


public:		//�����o�֐�
	bool InitializeDiscriptorHeap();
	bool CreateFromObj(string modelName);		//���f������
	bool LoadMaterial(const string &directoryPath, const string &filename);		//�}�e���A���ǂݍ���
	bool LoadTexture(const string &directoryPath, const string &filename);		//�e�N�X�`���ǂݍ���


	void Draw(ID3D12GraphicsCommandList *cmdList, ComPtr<ID3D12Resource> constBuffB0, ComPtr<ID3D12Resource> constBuffB1);								//�`��

	Material GetMaterial() { return material; }

private:		//�ÓI�����o�ϐ�
	static ID3D12Device *dev;								//�f�o�C�X
	/*static*/ CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;	//�V�F�[�_���\�[�X�r���[�n���h��(GPU)
	/*static*/ CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;	//�V�F�[�_���\�[�X�r���[�n���h��(CPU)
	static UINT descHandleIncrementSize;					//�f�X�N���v�^�T�C�Y

private:		//�����o�ϐ�
	ComPtr<ID3D12DescriptorHeap> descHeap;			//�e�N�X�`���f�X�N���v�^
	ComPtr<ID3D12Resource> vertBuff;			//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView;			//���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff;			//�C���f�b�N�X�o�b�t�@
	D3D12_INDEX_BUFFER_VIEW ibView;				//�C���f�b�N�X�o�b�t�@�r���[
	std::vector<VertexPosNormalUv> vertices;	//���_�f�[�^�z��
	std::vector<unsigned short> indices;		//�C���f�b�N�X�z��
	Material material;		//�}�e���A��

	ComPtr<ID3D12Resource> texBuff;	//�e�N�X�`���o�b�t�@

};

