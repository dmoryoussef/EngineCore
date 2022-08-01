#include "Engine.h"


#include "TestState.h"
#include "IsoTileMapTest.h"
#include "BuildingTest.h"
#include "BezierDemo.h"

void main()
{
	Engine *pEngine = new Engine();
	pEngine->start(new ConsoleWindow(150, 100, 4, 4), new BuildingTest());
	pEngine->run();
	delete pEngine;
}