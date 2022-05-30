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
	//今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	//（今の時間 - 前のフレームの時間）/ 周波数＝経過時間（秒単位）
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) {
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1);	//分解能をあげる
		Sleep(sleepTime);	//待機
		timeEndPeriod(1);	//戻す
	}
}

