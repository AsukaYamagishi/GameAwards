#pragma once
#include "Vector3.h"

//��
struct mSphere
{
	Vector3 position;	//���S���W
	float radius;		//���a
	unsigned color;		//�\���F

	//�R���X�g���N�^
	mSphere(const Vector3 &position, float radius, unsigned color);

	void draw();		//�`��
	void drawInfo(int x, int y, unsigned color);		//���̕\�����
};

//�{�b�N�X
struct Box
{
	//xyz�̂��ꂼ��̍ŏ��l�ƍő�l���f�[�^�Ƃ��Ď���������@
	Vector3 minPosition;	//�ŏ��l x,y,z
	Vector3 maxPosition;	//�ő�l x,y,z

	//���S���W�ƃT�C�Y�Ƃ��Ď���������@������
//	Vector3 position;	//���S���W
//	float halfSize[3];	//x�����̃T�C�Y/2�Ay�����̃T�C�Y/2�Az�����̃T�C�Y/2

	unsigned color;	//�\���F

	//�R���X�g���N�^
	Box(const Vector3 &minPosition, const Vector3 &maxPosition, unsigned color);

	void draw();		//�`��
	void drawInfo(int x, int y, unsigned color);
};

//�J�v�Z��
struct Capsule
{
	Vector3 startPosition;
	Vector3 endPosition;
	float radius;

	unsigned color;	//�\���F

	//�R���X�g���N�^
	Capsule(const Vector3 &startPosition, const Vector3 &endPosition, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};