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
	if (frameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		fps = (fps * 0.99f) + (0.01f / frameTime); // 平均fpsを計算
#ifdef _DEBUG
// デバッグ用(デバッガにFSP出す)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << fps << " FPS" << std::endl;
		// カウンタ付けて10回に1回出力、とかにしないと見づらいかもね
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG
	}
	timeStart = timeEnd;	//入れ替え
}

