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


class Object2D
{
#pragma region �\����
public: //�\����
	//���_�f�[�^�\����
	struct VertexPosUv
	{
		XMFLOAT3 pos; //xyz
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
private: //�ÓI�����o�ϐ�
	//�e�N�X�`���̍ő喇��
	static const int spriteSRVCount = 512; //�e�N�X�`���̍ő喇��
	//���_��
	static const int vertNum = 4;
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
	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource> texBuff[spriteSRVCount];
	//�ˉe�s��
	static XMMATRIX matProjection;

private: //�����o�ϐ�
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//Z���̉�]�p
	float rotation = 0.0f;
	//���[�J�����W
	XMFLOAT2 position = {};
	//�X�v���C�g���A����
	XMFLOAT2 size = { 100.0f,100.0f };
	//�A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld{};
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//���E���]
	bool isFlipX = false;
	//�㉺���]
	bool isFlipY = false;
	//�e�N�X�`���n�_
	XMFLOAT2 texBase = { 0,0 };
	//�e�N�X�`�����A����
	XMFLOAT2 texSize = { 100.0f,100.0f };
	
#pragma endregion

#pragma region �֐�
public: //�ÓI�����o�֐�
	//�ÓI������
	static bool StaticInit(ID3D12Device* dev, int window_width, int window_height);
	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);
	//�`��㏈��
	static void PostDraw();
	//�X�v���C�g����
	static Object2D* CreateSprite(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1,1,1,1 }, XMFLOAT2 anchorpoint = { 0.0f,0.0f }, bool isFlipX = false, bool isFlipY = false);
	//�e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

public: //�����o�֐�
	//�R���X�g���N�^
	Object2D(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlopY);
	//������
	bool Init();
	//�`��
	void Draw();
	//�p�x�̐ݒ�
	void SetRotation(float rotation);
	//���W�̐ݒ�
	void SetPosition(XMFLOAT2 position);
	//�T�C�Y�̐ݒ�
	void SetSize(XMFLOAT2 size);
	//�A���J�[�|�C���g�̐ݒ�
	void SetAnchorPoint(XMFLOAT2 anchorpoint);
	//���E���]�̐ݒ�
	void SetIsFlipX(bool isFlipX);
	//�㉺���]�̐ݒ�
	void SetIsFlipY(bool isFlipY);
	//�e�N�X�`���͈͐ݒ�
	void SetTextureRect(XMFLOAT2 texBase, XMFLOAT2 texSize);

private: //�����o�֐�
	//���_�f�[�^�]��
	void TransferVertices();
#pragma endregion

};

