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
	//Microosoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	mEffekseer* firework = nullptr;
	mEffekseer* explosion = nullptr;

	//火花関連のフラグ
	bool fwFlag = false;

	//爆破関連のフラグ
	bool elFlag = false;

public:
	~Effects();

public:

	/// <summary>
	/// 火花の初期化
	/// </summary>
	void FwInit(DirectXCommon* dxCommon, Camera* camera);

	/// <summary>
	/// 爆破の初期化
	/// </summary>
	void Elinit(DirectXCommon* dxCommon, Camera* camera);

	/// <summary>
	/// 火花のロード
	/// </summary>
	void FwLoad(bool& isfirework);

	/// <summary>
	/// 爆破のロード
	/// </summary>
	void ElLoad(bool& isexplosion);

	/// <summary>
	/// 火花の毎フレーム処理
	/// </summary>
	void FwUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput);

	/// <summary>
	/// 爆破の毎フレーム処理
	/// </summary>
	void ElUpdate(DirectXCommon* dxCommon, Camera* camera, Player* player, KeyboardInput* keyInput);

	/// <summary>
	/// 火花の描画処理
	/// </summary>
	void FwDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// 爆破の描画処理
	/// </summary>
	void ElDraw(DirectXCommon* dxCommon);
};

