#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include "WindowsAPI.h"

class DirectXCommon
{
private: 
	//Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //�����o�ϐ�
	//WindowsAPI�Ǘ�
	WindowsAPI* winApi;

	//Direct3D�֘A
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;


public: //�����o�֐�
	//������
	void Init(WindowsAPI* win);

	//�`��O����
	void PreDraw();

	//�`��㏈��
	void PostDraw();

	//�����_�[�^�[�Q�b�g�̃N���A
	void ClearRenderTarget();

	//�[�x�o�b�t�@�̃N���A
	void ClearDepthBuffer();

	//�f�o�C�X�̎擾
	ID3D12Device* GetDevice() { return dev.Get(); }

	//�`��R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetCommandList() { return cmdList.Get(); }

private: //���۔���
	//DXGI�f�o�C�X������
	bool InitDXGIDevice();

	//�X���b�v�`�F�[���̐���
	bool CreateSwapChain();

	//�R�}���h�֘A������
	bool InitCommand();

	//�����_�[�^�[�Q�b�g����
	bool CreateRenderTarget();

	//�[�x�o�b�t�@����
	bool CreateDepthBuffer();

	//�t�F���X����
	bool CreateFence();

};

