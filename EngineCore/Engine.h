using namespace std;

#include <stdio.h>
#include <Windows.h>
#include <WindowsX.h>
#include <string.h>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <xinput.h>

#include "ScreenBuffer.h"
#include "ConsoleScreenBuffer.h"

class Engine 
{
protected:
	bool m_bRunning;

	uint64_t nCountFreq;
	LARGE_INTEGER PrevCounter;

	float getDeltaTime()
	{
		//	convert to a class, to be able to handle multiple timers for testing
		LARGE_INTEGER CurrCounter;
		QueryPerformanceCounter(&CurrCounter);

		uint64_t nDeltaCounter = CurrCounter.QuadPart - PrevCounter.QuadPart;

		float fDelta = (1000 * nDeltaCounter) / nCountFreq;
		PrevCounter = CurrCounter;

		return fDelta;
	}

	void handleEvents() {}

	void update(float fDeltaTime) {}

	void render() {}

public:
	Engine()
	{
		m_bRunning = true;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		nCountFreq = Frequency.QuadPart;
	}

	~Engine() {}

	bool isRunning()
	{
		return m_bRunning;
	}

	void setup(ScreenBuffer *sb)
	{
	}

	void run()
	{
		float fSpeed = 1.0f;

		while (m_bRunning)
		{
			float fDeltaTime = getDeltaTime() * fSpeed;
			handleEvents();
			update(fDeltaTime);
			render();
		}
	}
};