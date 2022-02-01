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
	//コンストラクタに引数設定忘れずに
	Camera(XMFLOAT3 eye, XMFLOAT3 target = { 0,0,0 }, XMFLOAT3 up = { 0,1,0 });

	XMFLOAT3 eye; //視点座標
	XMFLOAT3 target; //注視点座標
	XMFLOAT3 up; //上ベクトル

	XMMATRIX matView; //カメラ情報


	void CameraUpdate(); //カメラ更新
};

