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
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <xinput.h>
#include <thread>
#include <mutex>

//	helper functions? move to new header?
template <typename T> string thingToString(T data)
{
	stringstream str;
	str << data;
	return str.str();
}

int random(int min, int max)
{
	return rand() % (max + 1) - min;
}

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

#include "ControllerInput.h"

#include "ConsoleWindow.h"
#include "Win32Window.h"

#include "ConsoleInputBuffer.h"
#include "ConsoleOutputBuffer.h"

enum GUI_STATE
{
	DEFAULT,
	MOUSE_OVER,
	LEFT_PRESSED,
	LEFT_RELEASED,
	RIGHT_PRESSED,
	RIGHT_RELEASED,
	LEFT_ACTIVE
};
#include "EntityComponentSystem.h"
#include "TileMapSystem.h"
#include "GUI.h"

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
	_UIComponent *m_pGUI;
	BaseNode* m_pSystems;


	EventListener events;
	thread eventThread;
	ControllerInput controllerInput;


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
		controllerInput.handleControllerInput();
		m_pInputBuffer->getConsoleInput();
		m_pInputBuffer->convertEvents();
		events.dispatchEvents();
	}

	void update(float fDeltaTime)
	{
		while (m_pData->isIterating())
		{
			m_pData->getCurrent()->baseUpdate(fDeltaTime);
		}

		m_pSystems->baseUpdate(fDeltaTime);

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
		PrevCounter({ 0, 0 }),
		events(),
		controllerInput(),

		m_pData(new BaseNode("Root")),
		m_pSystems(new BaseNode("Systems"))
		//eventThread(events)
	{
		m_bRunning = true;
		m_pWindow = NULL;
		m_pEngineBuffer = NULL;
		m_pInputBuffer = NULL;
		m_pGUI = NULL;
		
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		nCountFreq = Frequency.QuadPart;
	}

	~Engine() 
	{
		//	add to single container to delete at a group?
		delete m_pEngineBuffer;
		delete m_pInputBuffer;
		delete m_pWindow;
		delete m_pData;
		delete m_pSystems;
		delete m_pGUI;
	}

	bool isRunning()
	{
		return m_bRunning;
	}

	void setup(OutputWindow *sb, ConsoleInputBuffer *input)
	{
		//	seed random generator
		srand(0);
		m_pWindow = sb;
		m_pWindow->init();
		m_pEngineBuffer = new OutputBuffer(m_pWindow->getWidth(), m_pWindow->getHeight());
		m_pInputBuffer = input;
		m_pGUI = new _UIComponent(m_pEngineBuffer->getWidth(), m_pEngineBuffer->getHeight(), 0, 0);
		controllerInput.loadXInput();

		m_pSystems->addChild(new CollisionDetectionSystem(m_pData));
		m_pSystems->addChild(new EntityPhysicsSystem(m_pData));
		m_pSystems->addChild(new EntityFactory(m_pData));
		m_pSystems->addChild(new EntityCommandSystem(m_pData));

		CameraViewWindow *pCameraWindow = new CameraViewWindow(m_pEngineBuffer->getWidth() - 20, m_pEngineBuffer->getHeight(), 0, 0);
		m_pGUI->add(new ListExplorerWindow(m_pData));
		m_pData->add(pCameraWindow->getCamera());
		m_pGUI->add(pCameraWindow);
		
		

		//DefaultTileMap* mapA = new DefaultTileMap(8, 8);
		//mapA->setPosition(2, 2);
		//mapA->createCheckerMap();
		//m_pData->add(mapA);

		//DefaultTileMap* mapB = new DefaultTileMap(8, 8);
		//mapB->setPosition(14, 16);
		//mapB->createCheckerMap();
		//m_pData->add(mapB);

		//BaseNode *pControllerWidget = new BaseNode();
		//pControllerWidget->add(new ControllerWidget());
		//m_pData->add(pControllerWidget);

		//	change to detect controller first?
		m_pSystems->getChild<EntityFactory>()->createPlayer(0);
		m_pSystems->getChild<EntityFactory>()->createPlayer(1);
		
		
	}

	void addGUI(_UIComponent* pComponent)
	{
		m_pGUI->add(pComponent);
	}

	void addData(BaseNode *pNode)
	{
		m_pData->add(pNode);
	}

	virtual void start() {}

	void run()
	{
		float fSpeed = 1.0f;

		start();

		while (m_bRunning)
		{
			float fDeltaTime = getDeltaTime() * fSpeed;
			handleEvents();
			update(fDeltaTime);
			render();
		}
	}
};