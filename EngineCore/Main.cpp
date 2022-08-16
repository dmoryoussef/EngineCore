#include "Engine.h"


#include "TestState.h"
#include "IsoTileMapTest.h"
#include "BuildingTest.h"
#include "BezierDemo.h"
#include "RogueLikeDemo.h"

void main()
{
	Engine *pEngine = new Engine();
	pEngine->start(new ConsoleWindow(300, 150, 4, 4), new RogueLikeDemo());
	pEngine->run();
	delete pEngine;
}