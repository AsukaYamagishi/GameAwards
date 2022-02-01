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
	//Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //メンバ変数
	//WindowsAPI管理
	WindowsAPI* winApi;

	//Direct3D関連
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


public: //メンバ関数
	//初期化
	void Init(WindowsAPI* win);

	//描画前処理
	void PreDraw();

	//描画後処理
	void PostDraw();

	//レンダーターゲットのクリア
	void ClearRenderTarget();

	//深度バッファのクリア
	void ClearDepthBuffer();

	//デバイスの取得
	ID3D12Device* GetDevice() { return dev.Get(); }

	//描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() { return cmdList.Get(); }

private: //成否判定
	//DXGIデバイス初期化
	bool InitDXGIDevice();

	//スワップチェーンの生成
	bool CreateSwapChain();

	//コマンド関連初期化
	bool InitCommand();

	//レンダーターゲット生成
	bool CreateRenderTarget();

	//深度バッファ生成
	bool CreateDepthBuffer();

	//フェンス生成
	bool CreateFence();

};

