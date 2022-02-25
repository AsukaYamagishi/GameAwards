#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>
class PlayerBeam {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PlayerBeam();
	~PlayerBeam();
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);

	/// <summary>
	/// ñàÉtÉåÅ[ÉÄèàóù
	/// </summary>
	void Update();

	/// <summary>
	/// ï`âÊ
	/// </summary>
	void Draw();


public:

private:
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;

	ModelDraw* beam = nullptr;
	float move = 5.0f;



};