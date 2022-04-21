#pragma once
#include "SafeDelete.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include "Sprite.h"
#include "DebugText.h"
#include "Audio.h"
#include "KeyboardInput.h"
#include <vector>
#include "Player.h"
#include "Collision.h"
#include "FbxDraw.h"
#include "OBJObject.h"
#include"Boss.h"
#include "ParticleManager.h"
#include"Primitive.h"
#include "mCollision.h"

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
	KeyboardInput *input = nullptr;
	Audio *audio = nullptr;
	Camera *camera = nullptr;
	DebugText debugText;
	Sprite *spriteBG;

	//�Q�[���V�[���p
	float boss1HP_SizeX = 800;
	float boss1HP_SizeY = 32;
	Sprite* boss1HP_Red = nullptr;
	Sprite* boss1HP_Black = nullptr;
	float playerHPX = 128;
	float playerHPY = 128;
	int playerMaxHp = 10;
	Sprite* playerHP = nullptr;
	//���y
	Audio::SoundData soundData[3];
	Audio::SoundData soundSE[10];

	//�v���C���[
	Player *player;	
	//����
	OBJObject *weapon;
	//�X�e�[�W
	OBJObject *stage;
	OBJObject *skydome;
	//�{�X
	Boss *boss;



	//FBX���f��
	std::unique_ptr<FbxModel> testModel = nullptr;
	FbxDraw *testObject = nullptr;

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;


	//�G
	//vector<Enemy*> enemys;

	float angle = 3.14f / 2.0f;

	float radius = XMConvertToRadians(2.0f);
	int soundNo = 0;

public:
	//�Q�[���V�[���I���p
	bool gameEndFlag = false;

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
	//������
	void Init(DirectXCommon *dxCommon, KeyboardInput *input, Audio *audio);
	//���t���[���X�V����
	void Update();
	//�`��
	void Draw();
#pragma endregion

};

