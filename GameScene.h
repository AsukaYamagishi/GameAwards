#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include "ControllerInput.h"
#include <vector>
#include "Player.h"
#include "Collision.h"
#include "FbxDraw.h"
#include "OBJObject.h"
#include"Boss.h"
#include "ParticleManager.h"
#include"Primitive.h"
#include "mCollision.h"
#include"Effects.h"
class CollisionManager;
class FbxCollisionManager;
#include"Weapon.h"
using namespace Microsoft::WRL;
using namespace DirectX;

//�Q�[���V�[��
class GameScene
{
#pragma region �ϐ�
private: //�ÓI�����o�ϐ�
	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`��
	static const int debugTextTexNum = 0;

private: //�����o�ϐ�
	DirectXCommon *dxCommon = nullptr;
	KeyboardInput *keyInput = nullptr;
	ControllerInput* padInput = nullptr;
	Audio *audio = nullptr;
	Camera *camera = nullptr;
	DebugText debugText;
	Sprite *spriteBG;

	//�Q�[���V�[���p
	float boss1HP_SizeX = 800;
	float boss1HP_SizeY = 32;
	Sprite* boss1HP_Red = nullptr;
	Sprite* boss1HP_Black = nullptr;
	float playerHPX = 200;
	float playerHPY = 200;
	int playerMaxHp = 10;
	Sprite* playerHP = nullptr;
	Sprite* controler_rule = nullptr;
	Sprite* ketboard_rule = nullptr;
	Sprite* pose = nullptr;
	Sprite* pose_key = nullptr;
	bool poseFlag = false;
	//�E���镐��p���
	ModelDraw* arrow[5];
	bool DrawFlag[5];
	XMFLOAT3 arrowPos[5];
	float frame;
	float downTimer[5];
	bool isKeyBoard = false;
	bool isGamePad = true;
	//���y
	Audio::SoundData soundData[2];
	Audio::SoundData soundSE[7];

	//�R���W�����}�l�[�W���[�ɑ����Ăǂ̃p�[�c�A�Ώۂ��������Ă���̂���ۑ�����
	bool hit[20] = { false };
	//�Q�[���V�[���ɑ��݂���A�^�b�N����z��̃A�^�b�N�t���O��S�������
	bool attackFlag[10] = { false };
	//�v���C���[
	Player *player;	
	//����
	//OBJObject *weapon;
	Weapon* weapon = nullptr;
	//�X�e�[�W
	OBJObject *stage;
	OBJObject *skydome;
	//�{�X
	Boss *boss;

	//�Փ˃}�l�[�W��
	CollisionManager* collisionManager = nullptr;
	FbxCollisionManager* fbxcollisionManager = nullptr;

	//FBX���f��
	std::unique_ptr<FbxModel> testModel = nullptr;
	FbxDraw *testObject = nullptr;
	std::unique_ptr<FbxModel> testsphereModel = nullptr;
	FbxDraw* testsphereObject = nullptr;

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;

	/*std::unique_ptr<Effects>*/Effects* effects = nullptr;
	Effects* effects_2 = nullptr;

	bool isfirework = false;
	bool isexplosion = false;

	//�G
	//vector<Enemy*> enemys;

	float angle = 3.14f / 2.0f;

	float radius = XMConvertToRadians(2.0f);
	int soundNo = 0;
	int seNo = 0;

	int pressTimer = 0;
	float pressRadius = 0;
	bool fireFlag = false;
	float fireTimer = 0;

	bool fallFlag[6] = { 0 };

	bool fallFlag[6] = { 0 };

	bool LockFlag = false;
	bool cameraFlag = false;
	float cameraAngle;

	bool bgmflag = false;
public:
	//�Q�[���V�[���I���p
	bool gameFlag = false;
	bool winJudeg = false;
#pragma endregion

#pragma region �ÓI�����o�֐�

private: //�ÓI�����o�֐�
public: //�ÓI�����o�֐�
#pragma endregion

#pragma region �Q�b�^�[/�Z�b�^�[

#pragma endregion

#pragma region �����o�֐�

private: //�����o�֐�
public: //�����o�֐�
	//�R���X�g���N�^
	GameScene();
	//�f�X�g���N�^
	~GameScene();

	//
	void Finalize();

	//������
	void Init(DirectXCommon *dxCommon, KeyboardInput *keyInput, ControllerInput *padInput, Audio *audio);
	//���t���[���X�V����
	bool Update();
	//�`��
	void Draw();
#pragma endregion

};

