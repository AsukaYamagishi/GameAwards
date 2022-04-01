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
#include"mCollision.h"

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
	Sprite *sprite = nullptr;
	//���y
	Audio::SoundData soundData[3];

	//�v���C���[
	Player *player = nullptr;	
	//����
	OBJObject *weapon = nullptr;
	//�X�e�[�W
	OBJObject *stage = nullptr;
	OBJObject *skydome = nullptr;
	//�{�X
	Boss *boss = nullptr;

	//FBX���f��
	std::unique_ptr<FbxModel> testModel = nullptr;
	FbxDraw *testObject = nullptr;

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;


	//�G
	//vector<Enemy*> enemys;
	bool hitFlag[6] = { false };


	Vector3 meye = { 0,0,0 };
	Vector3 mtarget = { 0,0,0 };

	bool shakeflag = false;
	int shaketime = 0;
	Vector3 shakecamera = { 0,0,0 };	

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

