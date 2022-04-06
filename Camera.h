#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class Camera
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Camera(Vector3 eye, Vector3 target = { 0,0,0 }, Vector3 up = { 0,1,0 });

	//更新
	void Update();
	//ビュー行列更新
	void UpdateViewMatrix();
	//射影行列を更新
	void UpdateProjectionMatrix();


	static void Initialize(const XMFLOAT3 eye = { 0,100,100 });
	static void SetCam(Camera* cam);

	const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}
	static Camera* GetCam() { return NowMatView; }
	//ビュー行列取得
	XMMATRIX GetMatView() { return matView; }
	//プロジェクション行列取得
	XMMATRIX GetMatProj() { return matProjection; }
	


	//void TargetSpin(float length);

	Vector3 eye;			//視点座標
	Vector3 target;			//注視点座標
	Vector3 up;				//上方向ベクトル
	Vector3 front;			//正面ベクトル

	Vector3 angle;			//角度


protected:
	//今のカメラ
	static Camera* NowMatView;
	//ビュー行列
	XMMATRIX matView = XMMatrixIdentity();
	//射影行列
	XMMATRIX matProjection = XMMatrixIdentity();
	//ビュー射影行列
	XMMATRIX matViewProjection = XMMatrixIdentity();//現在ののカメラ
};

