//	Project Properties
//	>> Configuration Properties
//	>> Advanced
//	Charater Set: "Use Multi-Byte Character Set

using namespace std;

#include <stdio.h>
#include <Windows.h>
#include <WindowsX.h>
#include <string.h>
#include <map>
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <xinput.h>
#include <thread>
#include <mutex>

#define PI = 3.14159;
//	helper functions? move to new header?
template <typename T> string thingToString(T data)
{
	stringstream str;
	str << data;
	return str.str();
}

int random(int min, int max)
{
	return rand() % ((max + 1) - min);
}

float clamp(float value, float min, float max)
{
	if (value > max)
		value = max;
	if (value < min)
		value = min;

	return value;  
}

//	Geometry
#include "Vector2.h"
#include "Vector3.h"

#include "Triangle.h"

#include "2DMatrix.h"
#include "Polygon.h"

//	CORE STUFF
#include "EventListener.h"

#include "OutputBuffer.h"
#include "OutputWindow.h"
#include "Input.h"  

#include "Render2D.h"

#include "NodeCore.h"
#include "BaseNode.h"

#include "_Command.h"
#include "Events.h"	//	need a way to append this with user created events

//	Input/Output
#include "ControllerInput.h"
#include "ConsoleWindow.h"
#include "Win32Window.h"
#include "ConsoleOutputBuffer.h"

#include "PlatformInputHandler.h"
#include "Win32InputHandler.h"
#include "ConsoleInputHandler.h"

// these use control nodes, events, output buffer, renderer etc:
#include "SelectionSquare.h"
#include "DataTree.h"
#include "Bezier.h"
#include "Rect2D.h"	//	not correct - poly edit class uses this - restructure this later
#include "LiveEditPoly2D.h"
#include "ConnectingLine.h"

//	Systems
#include "EntityComponentSystem.h"
#include "GUI.h"
#include "TileMapSystem.h"

#include "GameState.h"

class Engine : public EventListener
{
protected:
	bool m_bRunning;
	float m_fGameSpeed;

	uint64_t nCountFreq;
	LARGE_INTEGER PrevCounter;

	OutputWindow *m_pWindow;
	OutputBuffer *m_pEngineBuffer;

	PlatformInputHandler *m_pInputHandler;

	BaseNode *m_pData;
	_UIComponent *m_pGUI;
	BaseNode* m_pSystems;

	EventListener events;
	thread eventThread;
	ControllerInput controllerInput;

	StateManager  *m_pStateManager;

	bool m_bPaused;

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

	void onEvent(_Event* pEvent)
	{
		if (pEvent->m_eType == KEYBOARD_EVENT)
		{
			if (pEvent->get<KeyboardEvent>()->isKeyDown(27))
				m_bRunning = false;

			if (pEvent->get<KeyboardEvent>()->isKeyDown('p'))
			{
				if (m_bPaused)
				{
					m_bPaused = false;
					m_fGameSpeed = 1.0;
				}
				else
				{
					m_bPaused = true;
					m_fGameSpeed = 0.0;
				}
			}
		}
	}

	void handleEvents() 
	{
		controllerInput.handleControllerInput();
		m_pInputHandler->processInput();
		events.dispatchEvents();
	}

	void update(float fDeltaTime)
	{
		m_pStateManager->update(m_pData, fDeltaTime);
		m_pGUI->baseUpdate(fDeltaTime);
		m_pData->baseUpdate(fDeltaTime);
		m_pSystems->baseUpdate(fDeltaTime);
	}

	void render() 
	{	
		m_pEngineBuffer->clear(FG_BLACK);
		//	a) render the current state to engine buffer
		m_pStateManager->render(m_pEngineBuffer);
		m_pGUI->render(m_pData, m_pEngineBuffer);
		//	b) render engine buffer to window buffer
		m_pWindow->renderToBuffer(m_pEngineBuffer);
		//	c) output buffer to window
		m_pWindow->outputToWindow();
	}

public:
	Engine() :
		m_bPaused(false),
		m_fGameSpeed(1.0),
		PrevCounter({ 0, 0 }),
		events(),
		controllerInput(),
		m_pStateManager(new StateManager()),
		m_pData(new BaseNode("Root")),
		m_pSystems(new BaseNode("Systems"))
		//eventThread(events)
	{
		m_bRunning = true;
		m_pWindow = NULL;
		m_pEngineBuffer = NULL;
		m_pInputHandler = NULL;
		m_pGUI = NULL;
		
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		nCountFreq = Frequency.QuadPart;

		registerListener(KEYBOARD_EVENT);
	}

	~Engine() 
	{
		//	add to single container to delete as a group?
		delete m_pEngineBuffer;
		delete m_pInputHandler;
		delete m_pWindow;
		delete m_pData;
		delete m_pSystems;
		delete m_pGUI;
	}

	bool isRunning()
	{
		return m_bRunning;
	}

	void start(GameState* pStartingState)
	{
		//srand(0);

		////	control window type (console or win32)
		////	m_pWindow = pStartingState->getWindow();
		//m_pWindow->init();
		//m_pEngineBuffer = new OutputBuffer(m_pWindow->getWidth(), m_pWindow->getHeight());
		//m_pInputHandler = new ConsoleInputHandler();

		//m_pGUI = new _UIComponent(m_pEngineBuffer->getWidth(), m_pEngineBuffer->getHeight(), 0, 0);
		//
		//controllerInput.loadXInput();

		//m_pSystems->addChild(new CollisionDetectionSystem(m_pData));
		//m_pSystems->addChild(new EntityPhysicsSystem(m_pData));
		//m_pSystems->addChild(new EntityFactory(m_pData));
		//m_pSystems->addChild(new EntityCommandSystem(m_pData));

		//m_pStateManager->setEngineBufferSize(m_pWindow->getWidth(), m_pWindow->getHeight());
		//m_pStateManager->setState(pStartingState);
		//m_pStateManager->start(m_pData, m_pSystems, m_pGUI);
	}

	void start(OutputWindow* ow, OutputBuffer *ob, PlatformInputHandler* ih, GameState *pStartingState)
	{
		srand(0);

		m_pWindow = ow;
		m_pWindow->init();
		m_pInputHandler = ih;

		ob->validate(ow->getWidth(), ow->getHeight());
		m_pEngineBuffer = ob;

		//	gui needs buffers based on the output buffer
		m_pGUI = new _UIComponent(m_pEngineBuffer->getWidth(), m_pEngineBuffer->getHeight(), 0, 0);

		controllerInput.loadXInput();

		m_pSystems->addChild(new CollisionDetectionSystem(m_pData));
		m_pSystems->addChild(new EntityPhysicsSystem(m_pData));
		m_pSystems->addChild(new EntityFactory(m_pData));
		m_pSystems->addChild(new EntityCommandSystem(m_pData));

		m_pStateManager->setEngineBufferSize(m_pWindow->getWidth(), m_pWindow->getHeight());
		m_pStateManager->setState(pStartingState);
		m_pStateManager->start(m_pData, m_pSystems, m_pGUI);
	}
	

	void run()
	{
		while (m_bRunning)
		{
			float fDeltaTime = getDeltaTime() * m_fGameSpeed;
			handleEvents();
			update(fDeltaTime);
			render();
		}
	}
};