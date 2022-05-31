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
#include "KeyboardInput.h"
#include "Player.h"

class Effects
{
private:
	//Microosoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	mEffekseer* firework = nullptr;

	bool fireFlag = false;
	float fireTimer = 0;
	bool fwFlag = false;

public:
	~Effects();

public:
	void FwInit(DirectXCommon* dxCommon, Camera* camera);

	/// <summary>
	/// �ΉԂ̖��t���[������
	/// </summary>
	void FwUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput);

	/// <summary>
	/// �ΉԂ̕`�揈��
	/// </summary>
	void FwDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// �ΉԂ̃��[�h
	/// </summary>
	void FwLoad(bool &isfirework);
};

