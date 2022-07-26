

class BuildingTile : public _Tile2D<BuildingTile>
{
private:
public:
	BuildingTile() :
		_Tile2D("BUILDING_TILE") {}
};

class BuildingMap : public _TileMap<BuildingTile>
{
private:
	//	building map specific algorithms
	//		flood fill
	//		tile events?
	//	mouse interation
	//		building, selecting tiles
	//		group selection

	void onEvent(_Event *pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				if (BuildingTile* pTile = getTile(pMouseEvent->getWorldPosition()))
				{
					if (pMouseEvent->getState().bLeftButtonDown)
					{
						//	pTile->setValue(1);
					}
				}
				break;
			}

			case SELECTIONSQUARE_EVENT:
			{
				if (!pEvent->get<SelectionSquareEvent>()->isActive())
				{
					SelectionSquareEvent* pSelectionEvent = pEvent->get<SelectionSquareEvent>();
					Vector2 vMin = pSelectionEvent->getMin();
					Vector2 vMax = pSelectionEvent->getMax();

					//	fill square

					//	draw square
					for (int y = vMin.Y; y < vMax.Y; y++)
					{
						for (int x = vMin.X; x < vMax.X; x++)
						{
							if (x == vMin.X || x == vMax.X - 1 || y == vMin.Y || y == vMax.Y - 1)
							if (BuildingTile* pTile = getWorldTile(x, y))
								pTile->setValue(1);
						}
					}

				}
				break;
			}
		}
	}

public:
	BuildingMap() :
		_TileMap({ 40, 40 }, "BUILDING_MAP")
	{
		setPosition(5, 5);
		createBoarderMap();
		registerListener(SELECTIONSQUARE_EVENT);
	}
};

class BuildingTest : public GameState
{
	//	this test state uses the prebuilt engine and entity components
	//	and adds some custom code inside its own class 
	//	to implement user defined behavior beyond the scope of the existing engine

	//	room - 2d array
	//	floors - list of rooms
	//	buildings - list of floors
	//		forms a stack: foundation, basement, 1st floor, 2nd floor, etc, attic, roof, etc

	//	flood fill to assign rooms
	//		heat contained in room
	//			produced by "hearth" type area
	//	"weather" kept out of rooms
	//		unless door/window is open?
	// 
	//	floor tile
	//	wall tile
	// 
	//	tiles that are interactive
	//		door - open/close
	//			allow light to pass
	//			allow pathfinding
	//		window - open/close
	//			allow light
	//			block pathfinding
	//		stairs
	//			increase/decrease z-value of entity
	
	//	render modes
	//		floor/level to render
	//		render inside only "active" building
	//		render only top of other buildings
	//		build mode - highlight important info


private:


public:
	BuildingTest() 
	{
		
	};


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(100, 100, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pGUI->addAtEnd(pCameraWindow);

		pData->add(new BuildingMap());
	}

	void render(OutputBuffer* pEngineBuffer)
	{

	}

	void update(BaseNode* pData, float fDeltaTime)
	{
		
	}
};