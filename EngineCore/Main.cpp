#include "Engine.h"

void main()
{
	Engine *pEngine = new Engine();
	pEngine->start(new ConsoleWindow(100, 100, 4, 4));
	pEngine->run();
	delete pEngine;
}