#include "Engine.h"


#include "TestState.h"
#include "IsoTileMapTest.h"
#include "BuildingTest.h"

void main()
{
	Engine *pEngine = new Engine();
	pEngine->start(new ConsoleWindow(200, 150, 4, 4), new BuildingTest());
	pEngine->run();
	delete pEngine;
}