#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>
class Player {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::��ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Player();
	~Player();
	void Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update(Camera camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	Vector3 GetNoAttackRotation() { return player->GetRotation() - attacktorota;}

public:
	ModelDraw *player = nullptr;
	int attacktime = 0;
	bool attack = false;
	bool enemyWepon = false;

private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;

	float move = 1.5f;
	int hp = 10;

public:
	Vector3 attacktorota = { 0.0f,0.0f,0.0f };

};