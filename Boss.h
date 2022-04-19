#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>

enum Parts
{
	head,
	body,
	rightarm,
	leftarm,
	rightleg,
	leftleg
};

class Boss {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Boss();
	~Boss();
	void Initialize(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio, ModelDraw* player);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//�U���q�b�g���̃_���[�W
	void HitDamage(int part, int damege = 0);
	
	//���ʂ������鏈��
	void Fall(int part);
	
	//�{�X�̈ړ�����
	void Move();
	
	//�{�X�̌������v���C���[�Ɍ����悤�ɂ��鏈��
	void Direction();
	
	//�v���C���[�̈ʒu���{�X�̍s��(�ړ���U��)�͈͓̔������肷�鏈��
	bool RangeJudge(float attackRange);
	
	//�{�X�̍U������
	void Attack();

public:
	ModelDraw *boss = nullptr;

	ModelDraw *head = nullptr;
	ModelDraw *body = nullptr;
	ModelDraw *leftarm = nullptr;
	ModelDraw *rightarm = nullptr;
	ModelDraw *leftleg = nullptr;
	ModelDraw *rightleg = nullptr;
	int hp = 18;
	int parthp[6] = { 3,3,3,3,3,3 };
	float angle = 0.0f;
	bool stopFlag = false;

private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	ModelDraw *player = nullptr;
	ModelDraw *bullet = nullptr;
	//�v���C���[�ւ̈ړ����x�̊���
	float move = 128.0f;
	//�U���p�ϐ�
	float coolTime = 100.0f;
	float chargeTime = 20.0f;
	bool attackFlag = false;
};