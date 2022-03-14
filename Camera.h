#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class Camera
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Camera(Vector3 eye, Vector3 target = { 0,0,0 }, Vector3 up = { 0,1,0 });

	//�X�V
	void Update();
	//�r���[�s��X�V
	void UpdateViewMatrix();
	//�ˉe�s����X�V
	void UpdateProjectionMatrix();


	static void Initialize(const XMFLOAT3 eye = { 0,100,100 });
	static void SetCam(Camera* cam);

	const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}
	static Camera* GetCam() { return NowMatView; }
	//�r���[�s��擾
	XMMATRIX GetMatView() { return matView; }
	//�v���W�F�N�V�����s��擾
	XMMATRIX GetMatProj() { return matProjection; }
	//���_���W�擾
	XMFLOAT3 GetEye() { return eye; }

	//void TargetSpin(float length);

	Vector3 eye;				//���_���W
	Vector3 target;			//�����_���W
	Vector3 up;				//������x�N�g��
	Vector3 front;			//���ʃx�N�g��

	Vector3 angle;			//�p�x
protected:
	//���̃J����
	static Camera* NowMatView;
	//�r���[�s��
	XMMATRIX matView = XMMatrixIdentity();
	//�ˉe�s��
	XMMATRIX matProjection = XMMatrixIdentity();
	//�r���[�ˉe�s��
	XMMATRIX matViewProjection = XMMatrixIdentity();//���݂̂̃J����


};

