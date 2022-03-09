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

#include "Vector2.h"
#include "Buffer.h"
#include "ConsoleBuffer.h"
#include "OutputWindow.h"
#include "ConsoleWindow.h"
#include "Win32Window.h"



#include "NodeCore.h"
#include "BaseNode.h"

#include "EventListener.h"
#include "Events.h"

#include "UIComponent.h"
#include "UILayout.h"

class Engine 
{
protected:
	bool m_bRunning;

	uint64_t nCountFreq;
	LARGE_INTEGER PrevCounter;

	OutputWindow *m_pWindow;
	Buffer* m_pEngineBuffer;

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

	void render() 
	{	
		//	render game data to screen buffer
		m_pWindow->renderToBuffer(m_pEngineBuffer);
		//	then
		//	output buffer to window
		m_pWindow->outputToWindow();
	}

public:
	Engine()
	{
		m_bRunning = true;
		m_pWindow = NULL;
		m_pEngineBuffer = NULL;

		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		nCountFreq = Frequency.QuadPart;
	}

	~Engine() 
	{
		delete m_pEngineBuffer;
		delete m_pWindow;
	}

	bool isRunning()
	{
		return m_bRunning;
	}

	void setup(OutputWindow *sb)
	{
		m_pWindow = sb;
		m_pWindow->init();
		m_pEngineBuffer = new Buffer(m_pWindow->getWidth(), m_pWindow->getHeight());
		m_pEngineBuffer->set("Hello", 10, 10, 15);
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