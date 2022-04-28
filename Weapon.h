#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>
class Weapon {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Weapon();
	~Weapon();
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);

	void Update();


	void Draw();


	//ŠeŽ²‚ÌÀ•W‚ð“¾‚éŠÖ”
	float GetPosX() { Vector3 pos = weapon->GetPos(); return pos.x; }
	float GetPosY() { Vector3 pos = weapon->GetPos(); return pos.y; }
	float GetPosZ() { Vector3 pos = weapon->GetPos(); return pos.z; }

public:
	ModelDraw* weapon = nullptr;

private:
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;
};