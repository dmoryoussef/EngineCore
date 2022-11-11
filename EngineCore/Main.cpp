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

void main()
{
	Engine *pEngine = new Engine();
	pEngine->start(new Win32Window(1080, 720, 4, 4), new Win32Demo());
	pEngine->run();
	delete pEngine;
}

//
//	TO-DO list:
// 
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