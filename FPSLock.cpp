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
	if (frameTime > 0.0) { // �o�ߎ��Ԃ�0���傫��(�������Ȃ��Ɖ��̌v�Z�Ń[�����Z�ɂȂ�Ǝv���)
		fps = (fps * 0.99f) + (0.01f / frameTime); // ����fps���v�Z
#ifdef _DEBUG
// �f�o�b�O�p(�f�o�b�K��FSP�o��)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << fps << " FPS" << std::endl;
		// �J�E���^�t����10���1��o�́A�Ƃ��ɂ��Ȃ��ƌ��Â炢������
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG
	}
	timeStart = timeEnd;	//����ւ�
}

