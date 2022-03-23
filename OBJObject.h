#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>

class OBJObject
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	OBJObject();
	~OBJObject();
	void Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio, ModelManager::ModelName modelNum);

	/// <summary>
	/// ñàÉtÉåÅ[ÉÄèàóù
	/// </summary>
	void Update();

	/// <summary>
	/// ï`âÊ
	/// </summary>
	void Draw();


public:
	ModelDraw *model = nullptr;

private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;

	float move = 5.0f;
	int hp = 10;




};