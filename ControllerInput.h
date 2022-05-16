#pragma once
#include "WindowsAPI.h"
#include <vector>

//�C���N���[�h�֌W
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�o�[�W�����w��
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


//�Q�[���p�b�h�E�{�^������
#define XBOX_INPUT_A      (0)  //A�{�^��
#define XBOX_INPUT_B      (1)  //B�{�^��
#define XBOX_INPUT_X      (2)  //X�{�^��
#define XBOX_INPUT_Y      (3)  //Y�{�^��
#define XBOX_INPUT_LB     (4)  //LB�{�^��
#define XBOX_INPUT_RB     (5)  //RB�{�^��
#define XBOX_INPUT_SELECT (6)  //SELECT�{�^��
#define XBOX_INPUT_START  (7)  //START�{�^��
#define XBOX_INPUT_LSTICK (8)  //���X�e�B�b�N��������
#define XBOX_INPUT_RSTICK (9)  //�E�X�e�B�b�N��������
#define XBOX_INPUT_LEFT   (34) //�\���L�[��
#define XBOX_INPUT_RIGHT  (32) //�\���L�[�E
#define XBOX_INPUT_UP     (35) //�\���L�[��
#define XBOX_INPUT_DOWN   (33) //�\���L�[��

//�Q�[���p�b�h�̃X�e�B�b�N����
#define INPUT_AXIS_LX  (0x01) //���X�e�B�b�N��X��
#define INPUT_AXIS_LY  (0x02) //���X�e�B�b�N��Y��
#define INPUT_AXIS_LZ  (0x04) //���X�e�B�b�N��Z���iLT�ERT�j
#define INPUT_AXIS_RX (0x08) //���X�e�B�b�N��X��]���i�E�X�e�B�b�N��X���j
#define INPUT_AXIS_RY (0x10) //���X�e�B�b�N��Y��]���i�E�X�e�B�b�N��Y���j
#define INPUT_AXIS_RZ (0x20) //���X�e�B�b�N��Z��]��


class ControllerInput final
{
	//�V���O���g���p�^�[���ݒ�
private: //�R���X�g���N�^�E�f�X�g���N�^private��
	ControllerInput(){}
	~ControllerInput(){}
public: //�R���X�g���N�^�֌W�̐ݒ�
	//�R�s�[�R���X�g���N�^�̍폜
	ControllerInput(const ControllerInput&) = delete;
	//������Z�q�̍폜
	const ControllerInput& operator=(const ControllerInput&) = delete;
	//�C���X�^���X�̎擾
	static ControllerInput* GetInstance();

private: //�G�C���A�X
	template<class T> using vector = std::vector<T>;


public: //�ÓI�����o�֐�
	//������
	static void Init();
	//�X�V
	static void Update();

	//�Q�[���p�b�h�̏�Ԏ擾
	static DIJOYSTATE GetGamepadState(const size_t& padNo = 0);
	//�{�^����������Ă��邩
	static bool IsPadButton(const size_t& button, const size_t& padNo = 0);
	//�{�^����������Ă���Ԃ̎��Ԃ̎擾
	static size_t GetPadButtonPress(const size_t& button, const size_t& padNo = 0);
	//�{�^���𗣂��Ă���Ԃ̎��Ԃ̎擾
	static size_t GetPadButtonRelease(const size_t& button, const size_t& padNo = 0);
	//�{�^���������ꂽ���i�����ꂽ�u�ԁj
	static bool IsPadButtonTrigger(const size_t& button, const size_t& padNo = 0);
	//�{�^�����痣�ꂽ��
	static bool IsPadButtonRelease(const size_t& button, const size_t& padNo = 0);
	//�X�e�B�b�N�ɓ��͂����邩
	static float IsPadStick(const size_t& stickAxis, const float& deadzone, const size_t& padNo = 0);

private:
	//�Q�[���p�b�h�f�o�C�X�̐���
	static HRESULT CreateGamepadDevice();
	//�f�o�C�X�񋓃R�[���o�b�N�֐�
	static BOOL CALLBACK EnumGamepadCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	//�v���p�e�B�ݒ�
	static HRESULT SetupGamepadProperty(const size_t& padNo = 0);


public: //�萔
	static const LONG zoneMax; //�X�e�B�b�N���͏��
	static const LONG zoneMin; //�X�e�B�b�N���͉���

private: //�ÓI�����o�ϐ�
	static IDirectInput8* dinput;
	static vector<IDirectInputDevice8*> devGamepad; //�Q�[���p�b�h�f�o�C�X
	static vector<DIJOYSTATE> gamepad; //�Q�[���p�b�h�̌��݂̏��
	static vector<DIJOYSTATE> oldgamepad; //1�t���[���O�̃Q�[���p�b�h�̏��
	static size_t pressCount[64]; //�����Ă��鎞��
	static size_t releaseCount[64]; //�����Ă��鎞��

};

