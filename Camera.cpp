#include "Camera.h"
#include "WindowsAPI.h"
#include "yMath.h"

Camera* Camera::NowMatView = nullptr;

Camera::Camera(Vector3 eye, Vector3 target, Vector3 up)
{
	this->eye = eye;
	this->target = target;
	this->up = up;
	//�r���[�s��̌v�Z
	UpdateViewMatrix();
	//�ˉe�s��̌v�Z
	UpdateProjectionMatrix();
	//�r���[�v���W�F�N�V��������
	matViewProjection = matView * matProjection;
}

void Camera::Update()
{
	//�r���[�s��X�V
	UpdateViewMatrix();
	//�r���[�v���W�F�N�V�����X�V
	UpdateProjectionMatrix();

	//�r���[�v���W�F�N�V��������
	matViewProjection = matView * matProjection;

}

void Camera::UpdateViewMatrix()
{
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));
}

void Camera::UpdateProjectionMatrix()
{
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)WindowsAPI::window_width / WindowsAPI::window_height,
		0.1f, 50000.0f
	);
}

void Camera::Initialize(const XMFLOAT3 eye)
{
	NowMatView = new Camera(Vector3{ eye });
	NowMatView->matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&Vector3{ 0,0,0 }),
		XMLoadFloat3(&Vector3{ 0,1,0 })
	);
}

void Camera::SetCam(Camera* cam)
{
	NowMatView = cam;
}

//void Camera::TargetSpin(float length)
//{
//	//�W���o�����b�N���
//	if (angle.x > 89) {
//		angle.x = 89;
//	}
//	if (angle.x < -89) {
//		angle.x = -89;
//	}
//	angle.y = yMath::ChangeDegree360(angle.y);
//
//	Vector3 Rad = angle * (XM_PI / 180.0f);
//	//��]�s��ɕϊ�
//	XMMATRIX matRotate = XMMatrixRotationRollPitchYaw(Rad.x, Rad.y, Rad.z);
//	front = matRotate.r[2];
//
//	eye = target - front * length;
//
//}
