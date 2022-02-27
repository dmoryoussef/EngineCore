#include "Engine.h"

void main()
{
	Engine *pEngine = new Engine();
	pEngine->setup(new ConsoleScreenBuffer(100, 100, 8, 8));

}