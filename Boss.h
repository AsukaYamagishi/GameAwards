#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>
class Boss {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Boss();
	~Boss();
	void Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio);

	/// <summary>
	/// ñàÉtÉåÅ[ÉÄèàóù
	/// </summary>
	void Update();

	/// <summary>
	/// ï`âÊ
	/// </summary>
	void Draw();


public:
	ModelDraw *boss = nullptr;

	ModelDraw *head = nullptr;
	ModelDraw *body = nullptr;
	ModelDraw *leftaram = nullptr;
	ModelDraw *rightaram = nullptr;
	ModelDraw *leftleg = nullptr;
	ModelDraw *rightleg = nullptr;

private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;

	float move = 5.0f;
	int hp = 10;

};