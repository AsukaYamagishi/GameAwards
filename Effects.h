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
	mEffekseer* explosion = nullptr;

	//�ΉԊ֘A�̃t���O
	bool fwFlag = false;

	//���j�֘A�̃t���O
	bool elFlag = false;

public:
	~Effects();

public:

	/// <summary>
	/// �ΉԂ̏�����
	/// </summary>
	void FwInit(DirectXCommon* dxCommon, Camera* camera);

	/// <summary>
	/// ���j�̏�����
	/// </summary>
	void Elinit(DirectXCommon* dxCommon, Camera* camera);

	/// <summary>
	/// �ΉԂ̃��[�h
	/// </summary>
	void FwLoad(bool& isfirework);

	/// <summary>
	/// ���j�̃��[�h
	/// </summary>
	void ElLoad(bool& isexplosion);

	/// <summary>
	/// �ΉԂ̖��t���[������
	/// </summary>
	void FwUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput);

	/// <summary>
	/// ���j�̖��t���[������
	/// </summary>
	void ElUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput);

	/// <summary>
	/// �ΉԂ̕`�揈��
	/// </summary>
	void FwDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// ���j�̕`�揈��
	/// </summary>
	void ElDraw(DirectXCommon* dxCommon);
};

