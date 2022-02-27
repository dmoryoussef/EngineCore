#include "Engine.h"

void main()
{
	Engine *pEngine = new Engine();
	pEngine->setup(new Win32Window(100, 100, 8, 8));
	pEngine->run();
}