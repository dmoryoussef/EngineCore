#include "Engine.h"

#include "TestState.h"
#include "IsoTileMapTest.h"
#include "BuildingTest.h"
#include "BezierDemo.h"
#include "RogueLikeDemo.h"
#include "LivePolyEditor.h"
#include "Vector2Demo.h"
#include "TileLightingDemo.h"
#include "BehaviorTreeDemo.h"
#include "DataTreeDemo.h"
#include "RectDemo.h"
#include "Win32Demo.h"
#include "PerlinDemo.h"
#include "InfiniteTileMapDemo.h"

void main()
{
	Engine engine;

	//	window, buffer, inputhandler should all be packaged together

	//engine.start(new Win32Window(720, 480, 1, 1),
	//			 &Win32OutputBuffer,	//	new Win32Buffer(720, 480, 4, 4),
	//			 new Win32InputHandler(),
	//			 new Win32Demo());

	engine.start(new ConsoleWindow(200, 150, 1, 1),
				 new ConsoleOutputBuffer(200, 150, 1, 1),
				 new ConsoleInputHandler(),
				 new BuildingTest());
	engine.run();
}

//
//	TO-DO list:
// 
//	Win32 *Everything*
//	Entity-TileMap collision and interaction
//	Quad-Tree
//	More Steering behaviors
//	More AI behaviors
//	Pathfinding
//	Implement A*, breadth-first search, depth-first search
//	Floodfill
//	Roads/paths
//	Tilemap chunk loading
//	"Infinite" world generation
//	
//	Entities
//		plants	
//	Climate/Weather maps
//	1D/2D Perlin noise
//	Water
//		lakes/ponds
//		oceans
//		rivers
//	