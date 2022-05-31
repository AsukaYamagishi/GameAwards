#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include "KeyboardInput.h"
#include "Audio.h"
#include "ModelDraw.h"
#include "ModelManager.h"

#include <DirectXMath.h>
#include"FbxDraw.h"
enum Parts
{
	head,
	body,
	rightarm,
	leftarm,
	rightleg,
	leftleg
};

enum Sound
{
	Charge = 0,
	Shot
};

enum AttackType {
	NONE,
	BEAM,
	PRESS,
	RUSH
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
	void Initialize(DirectXCommon* dxCommon, KeyboardInput* input, Audio* audio, ModelDraw* player);

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
	//�r�[���U��
	void BeamAttack();

	//�v���X�U��
	void PressAttack();

	//�ːi�U��
	void Rush();

	//�_���[�W����
	int damage(float weaponATK);

	//�ʒu�␳
	void PosCorrection();

	bool GetAttackFlag() { return attackFlag; }
	int GetAttackTime() { return attackTime; }
	int GetAttackType() { return attackType; }
public:
	ModelDraw* boss = nullptr;

	ModelDraw* head = nullptr;
	ModelDraw* body = nullptr;
	ModelDraw* leftarm = nullptr;
	ModelDraw* rightarm = nullptr;
	ModelDraw* leftleg = nullptr;
	ModelDraw* rightleg = nullptr;
	
	

	FbxDraw* fbxparantboss = nullptr;

	//FbxDraw* head = nullptr;
	//FbxDraw* body = nullptr;
	FbxDraw* fbxRightArm = nullptr;
	FbxDraw* fbxLeftArm = nullptr;
	FbxDraw* fbxLeftLeg = nullptr;
	FbxDraw* fbxRightLeg = nullptr;
	FbxDraw* fbxLeftArmWalk = nullptr;
	FbxDraw* fbxRightArmWalk = nullptr;
	FbxDraw* fbxLeftArmPress = nullptr;
	FbxDraw* fbxRightArmPress = nullptr;

	//std::unique_ptr<FbxModel> headModel;
	//std::unique_ptr<FbxModel> bodyModel;
	std::unique_ptr<FbxModel> fbxRightArmModel;
	std::unique_ptr<FbxModel> fbxLeftArmModel;
	std::unique_ptr<FbxModel> fbxRightLegModel;
	std::unique_ptr<FbxModel> fbxLeftLegModel;

	std::unique_ptr<FbxModel> fbxRightArmWarkModel;
	std::unique_ptr<FbxModel> fbxLeftArmWarkModel;
	std::unique_ptr<FbxModel> fbxRightArmPressModel;
	std::unique_ptr<FbxModel> fbxLeftArmPressModel;


	const int maxhp = 18;
	int hp = maxhp;
	int parthp[6] = { 3,3,3,3,3,3 };
	float angle = 0.0f;
	float watch = 0.0f;
	float watchB = 0.0f;
	bool stopFlag = false;
	int attackType = NONE;

	int timer = 0;
private:
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	ModelDraw *player = nullptr;
	ModelDraw *bullet = nullptr;
	ModelDraw* shockWave = nullptr;
	//�v���C���[�ւ̈ړ����x�̊���
	float move = 128.0f;

	//�U���p�ϐ�
	//���ʕϐ�
	Vector3 oldBossPos = { 0, 0, 0 };
	Vector3 oldPlayerPos = { 0, 0, 0 };
	Vector3 correctionPos = { 0, 0, 0 };
	float coolTime = 100.0f;
	float chargeTime = 30.0f;
	bool attackFlag = false;
	float shakePosX = 0.0f;
	float shakePosZ = 0.0f;
	bool noneLeg = false;
	bool noneArm = false;
	bool noneLeftArm = false;
	bool noneRightArm = false;
	//�r�[���U���p�ϐ�
	Vector3 bulletPos = { 0, 0, 0 };
	Vector3 bulletScale = { 0, 0, 0 };
	float attackTime = 100.0f;
	//�v���X�U���p�ϐ�
	Vector3 pressPos = { 0, 0, 0 };
	Vector3 shockPos = { 0, 0, 0 };
	Vector3 shockScale = { 0, 0, 0 };
	float pressPower = 0.0f;
	float pressWaitTime = 10.0f;
	bool shockFlag = false;
	bool pressWaitFlag = false;
	//�ːi�U���p�ϐ�
	int rushCount = 0;
	
	//SE�p�ϐ�
	Audio::SoundData soundSE[2];
};