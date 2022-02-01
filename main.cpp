#include<Windows.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dx12.h>
#include<vector>
#include<string>
#include<DirectXMath.h>
#include<d3dcompiler.h>

#include"WindowsAPI.h"  //�E�B���h�E�\��
#include"KeyboardInput.h"  //�L�[�{�[�h����
#include"Sprite.h"  //2D�摜(sprite)
#include"Object3D.h"  //3D�I�u�W�F�N�g(Object)
#include"Audio.h" //���y�Đ�
#include"DebugText.h" //�f�o�b�O�e�L�X�g
#include"DirectXCommon.h"
#include"SafeDelete.h"
#include"GameScene.h"
#include"ModelObj.h"
#include"Title.h"
#include "SceneManager.h"

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xaudio2.lib")


using namespace DirectX;
using namespace Microsoft::WRL;


//Windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	//�E�B���h�E
	WindowsAPI* win = nullptr;
	win = new WindowsAPI();
	win->WindowCreate();

	MSG msg{}; //���b�Z�[�W



#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif // _DEBUG


	////DirectX�����������@��������
		
	//DirectX�ėp����
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Init(win);

	//������
	Audio* audio = nullptr;
	audio = new Audio();
	audio->Init();

	
	////DirectInput(����)����������
	KeyboardInput* input = KeyboardInput::GetInstance();
	input->Init(win->hwnd);

	////DirectX�����������@�����܂�


	////�`�揉���������@��������
#pragma region �X�v���C�g/3D�I�u�W�F�N�g�ÓI������
	// �X�v���C�g�ÓI������
	if (!Object2D::StaticInit(dxCommon->GetDevice(), WindowsAPI::window_width, WindowsAPI::window_height)) {
		assert(0);
		return 1;
	}

	// 3D�I�u�W�F�N�g�ÓI������
	if (!Object3D::StaticInit(dxCommon->GetDevice(), WindowsAPI::window_width, WindowsAPI::window_height)) {
		assert(0);
		return 1;
	}
#pragma endregion

#pragma region 3D���f���ÓI������
	/*if (!ModelObj::StaticInit(dxCommon->GetDevice(), WindowsAPI::window_width, WindowsAPI::window_height)) {
		assert(0);
		return 1;
	}*/
#pragma endregion
	

#pragma region �Q�[���V�[��������
	
	SceneManager* sceneManager = nullptr;
	sceneManager = new SceneManager();
	sceneManager->Init(dxCommon, input, audio);

#pragma endregion
	
	////�`�揉���������@�����܂�

	while (true) //�Q�[�����[�v
	{
	#pragma region ���b�Z�[�W�m�F
		//���b�Z�[�W�����邩
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg); //�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (msg.message == WM_QUIT)
		{
			break;
		}
	#pragma endregion

		////////////////DirectX���t���[������ ��������

	#pragma region �X�V����
		input->Update();
		sceneManager->Update();
#pragma endregion
		
	#pragma region �`��R�}���h
		//�`��O����
		dxCommon->PreDraw();

		sceneManager->Draw();
		
		//�`��I��
		dxCommon->PostDraw();
#pragma endregion

		////////////////DirectX���t���[������ �����܂�
	}
	//�N���X(new)�̏���
	safe_delete(dxCommon);
	safe_delete(audio);
	safe_delete(sceneManager);

	//�E�B���h�E�N���X��o�^����
	WindowsAPI::WindowClassRelease();
	safe_delete(win);

	return 0;
}

