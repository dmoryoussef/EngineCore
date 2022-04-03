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
#include <thread>

//	helper functions? move to new header?
template <typename T> string thingToString(T data)
{
	stringstream str;
	str << data;
	return str.str();
}

#include "Vector2.h"
#include "Vector3.h"

//	CORE STUFF
#include "EventListener.h"
#include "OutputBuffer.h"
#include "OutputWindow.h"
#include "Input.h"  //	inherits eventlistener, posts console events to event system
#include "Render2D.h"
//	ConsoleRenderer.h
//	Win32Renderer.h

#include "NodeCore.h"
#include "BaseNode.h"

#include "Events.h"	//	need a way to append this with user created events

#include "ConsoleWindow.h"
#include "Win32Window.h"

#include "ConsoleInputBuffer.h"
//	WindowsInputBuffer.h
#include "ConsoleOutputBuffer.h"

//	derive from here

//	OBJECTS AND STUFF
//	EntityComponentSystem, TileMapSystem, etc
#include "_EntityComponent.h"
#include "Transform.h"

#include "UIComponent.h"
#include "UILayout.h"
#include "UIWindow.h"
#include "WorldViewWindow.h"
#include "CameraViewWindow.h"

#include "_Tile2D.h"
#include "_TileMap.h"


class Engine
{
protected:
	bool m_bRunning;

	uint64_t nCountFreq;
	LARGE_INTEGER PrevCounter;

	OutputWindow *m_pWindow;
	OutputBuffer *m_pEngineBuffer;

	ConsoleInputBuffer *m_pInputBuffer;

	BaseNode *m_pData;
	_Window *m_pGUI;

	EventListener events;
	thread eventThread;

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

	void handleEvents() 
	{
		m_pInputBuffer->getConsoleInput();
		m_pInputBuffer->convertEvents();
		//m_pInputBuffer->dispatchEvents();
		//events.dispatchEvents();
	}

	void update(float fDeltaTime)
	{
		while (m_pData->isIterating())
		{
			m_pData->getCurrent()->update(fDeltaTime);
		}
	}

	void render() 
	{	
		//	a) render game data to engine buffer
		m_pGUI->render(m_pData, m_pEngineBuffer);
		//	b) render engine buffer to window
		m_pWindow->renderToBuffer(m_pEngineBuffer);
		//	c) output buffer to window
		m_pWindow->outputToWindow();
	}

public:
	Engine() :
		events(),
		eventThread(events)
	{
		m_bRunning = true;
		m_pWindow = NULL;
		m_pEngineBuffer = NULL;
		m_pInputBuffer = NULL;
		m_pData = new BaseNode("Root");
		m_pGUI = NULL;
		

		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		nCountFreq = Frequency.QuadPart;
	}

	~Engine() 
	{
		delete m_pEngineBuffer;
		delete m_pInputBuffer;
		delete m_pWindow;
		delete m_pData;
		delete m_pGUI;
	}

	bool isRunning()
	{
		return m_bRunning;
	}

	void setup(OutputWindow *sb, ConsoleInputBuffer *input)
	{
		m_pWindow = sb;
		m_pWindow->init();
		m_pEngineBuffer = new OutputBuffer(m_pWindow->getWidth(), m_pWindow->getHeight());
		m_pInputBuffer = input;

		CameraViewWindow *pCameraWindow = new CameraViewWindow(m_pEngineBuffer->getWidth(), m_pEngineBuffer->getHeight(), 0, 0);
		m_pGUI = pCameraWindow;
		m_pData->add(pCameraWindow->getCamera());

		DefaultTileMap* pMap2 = new DefaultTileMap(4, 4);
		//	pMap2->setPosition(2, 2);
		m_pData->add(pMap2);

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