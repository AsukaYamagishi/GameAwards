#include "FPSLock.h"

FPSLock::FPSLock()
{
	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);
}

FPSLock::~FPSLock()
{
}

void FPSLock::Update()
{
	//���̎��Ԃ��擾
	QueryPerformanceCounter(&timeEnd);
	//�i���̎��� - �O�̃t���[���̎��ԁj/ ���g�����o�ߎ��ԁi�b�P�ʁj
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1);	//����\��������
		Sleep(sleepTime);	//�ҋ@
		timeEndPeriod(1);	//�߂�
	}
}

