#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include"PlayerModel.h"
#include"PlayerActiveModel.h"
//#include"bullet2.h"
#include <DirectXMath.h>
class Player {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Player();
	~Player();
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//���ڃv���C���[�؂�ւ�
	void ChangeActivePlayer();

	int GetActiveNumber() { return activePlayer; }
	//�v���C���[�̍��W�擾
	XMFLOAT3 GetActivePos() { return playerObj[activePlayer]->GetPosition(); }

public:

private:
	DirectXCommon* dxCommon = nullptr;
	KeyboardInput* input = nullptr;
	Audio* audio = nullptr;

	

	//�v���C���[
	PlayerModel* playerObj[4];
	//�e�𔭎˂ł���v���C���[
	PlayerActiveModel* activePlayerObj;
	//�����ɑΉ�
	enum Direction {
		DOWN, //0
		LEFT, //1
		UP,   //2
		RIGHT,//3
		MAX,  //�ő吔
	};
	Direction direction = DOWN;

	//���ڃv���C���[�؂�ւ�
	int activePlayer = 0;
	

	//�v���C���[�̃X�s�[�h
	float playerSpeed[4] = { 0.5f,-0.5f,-0.5f,0.5f };
	


};