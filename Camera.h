#pragma once
#include<d3d12.h>
#include<d3dx12.h>
#include<DirectXMath.h>

#include"Object2D.h"
#include"Object3D.h"

using namespace DirectX;

class Camera
{
public:
	//�R���X�g���N�^�Ɉ����ݒ�Y�ꂸ��
	Camera(XMFLOAT3 eye, XMFLOAT3 target = { 0,0,0 }, XMFLOAT3 up = { 0,1,0 });

	XMFLOAT3 eye; //���_���W
	XMFLOAT3 target; //�����_���W
	XMFLOAT3 up; //��x�N�g��

	XMMATRIX matView; //�J�������


	void CameraUpdate(); //�J�����X�V
};

