#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include"mEffekseer.h"
#include"DirectXCommon.h"
#include"Camera.h"
#include "Player.h"

class Effects
{
private:
	//Microosoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	mEffekseer* effect = nullptr;

public:
	~Effects();

public:
	void Initialize(DirectXCommon* dxCommon, Camera* camera);
	void Update(DirectXCommon* dxCommon, Camera* camera, Player* player);
	void Draw(DirectXCommon* dxCommon);
};

