#pragma once
#include "Vector3.h"

//球
struct mSphere
{
	Vector3 position;	//中心座標
	float radius;		//半径
	unsigned color;		//表示色

	//コンストラクタ
	mSphere(const Vector3 &position, float radius, unsigned color);

	void draw();		//描画
	void drawInfo(int x, int y, unsigned color);		//球の表示情報
};

//ボックス
struct Box
{
	//xyzのそれぞれの最小値と最大値をデータとして持たせる方法
	Vector3 minPosition;	//最小値 x,y,z
	Vector3 maxPosition;	//最大値 x,y,z

	//中心座標とサイズとして持たせる方法もある
//	Vector3 position;	//中心座標
//	float halfSize[3];	//x方向のサイズ/2、y方向のサイズ/2、z方向のサイズ/2

	unsigned color;	//表示色

	//コンストラクタ
	Box(const Vector3 &minPosition, const Vector3 &maxPosition, unsigned color);

	void draw();		//描画
	void drawInfo(int x, int y, unsigned color);
};

//カプセル
struct Capsule
{
	Vector3 startPosition;
	Vector3 endPosition;
	float radius;

	unsigned color;	//表示色

	//コンストラクタ
	Capsule(const Vector3 &startPosition, const Vector3 &endPosition, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};