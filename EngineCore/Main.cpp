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

void main()
{
	Engine *pEngine = new Engine();
	pEngine->start(new ConsoleWindow(300, 150, 4, 4), new BehaviorTreeDemo());
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