#pragma once
#include <fstream>
#include <sstream>
#include <windows.h>
#include <timeapi.h>
#pragma comment(lib,"winmm.lib")

class FPSLock
{
public:
	FPSLock();
	~FPSLock();

	void Update();

private:
	const float MIN_FREAM_TIME = 1.0f / 60;
	float frameTime = 0;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	float fps = 0;;
};

