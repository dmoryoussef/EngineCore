

class BuildingTile : public _Tile2D<BuildingTile>
{
private:
public:
	BuildingTile() :
		_Tile2D("BUILDING_TILE") {}

};

class Floor
{
private:
	Vector2 Min;
	Vector2 Max;
	BuildingTile *map;

public:
	Floor(Vector2 min, Vector2 max) :
		Min(min),
		Max(max),
		map(NULL)
	{
		map = new BuildingTile[(Max.X - Min.X) * (Max.Y - Min.Y)];
	}

	void render()
	{
		for (int y = Min.Y; y < Max.Y; y++)
		{
			for (int x = Min.X; x < Max.X; x++)
			{
				//	draw to main building tilemap
			}
		}
	}
};

class Building : public BaseNode
{
private:
	Floor* current;
	vector<Floor> Floors;

public:
	Building(Vector2 min, Vector2 max) :
		Min(min),
		Max(max)
	{};

	Vector2 Min;
	Vector2 Max;

	void render()
	{
		//	render current floor 
		//	default: top floor?
	}
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

	int nCurrentBuildMode;
	vector<BuildingTile*> SelectedTiles;
	vector<Building*> Buildings;

	bool isValidPlacement(Vector2 vMin, Vector2 vMax)
	{
		for (auto b : Buildings)
		{
			if (vMax.X > b->Min.X)
				if (vMin.X < b->Max.X)
					if (vMax.Y > b->Min.Y)
						if (vMin.Y < b->Max.Y)
							return false;
		}

		return true;
	}

	void onEvent(_Event *pEvent)
	{
		_TileMap::onEvent(pEvent);

		switch (pEvent->m_eType)
		{
			case GUI_EVENT:
			{
				if (pEvent->m_eType == GUI_EVENT)
				{
					if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getState() == LEFT_RELEASED)
					{
						if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getText() == "POINT")
						{
							nCurrentBuildMode = 0;
						}

						if (pEvent->get<GuiEvent>()->getComponent<UIButton>()->getText() == "QUAD")
						{
							nCurrentBuildMode = 1;
						}
					}
				}
				break;
			}

			case MOUSEWORLD_EVENT:
			{
				MouseWorldEvent* pMouseEvent = pEvent->get<MouseWorldEvent>();
				if (BuildingTile* pTile = getWorldTile(pMouseEvent->getWorldPosition().X, pMouseEvent->getWorldPosition().Y))
				{
					if (pMouseEvent->getState().bRightButtonDown)
					{
						pTile->setValue(0.1);
					}
				}
				break;
			}

			case SELECTIONSQUARE_EVENT:
			{
				SelectionSquareEvent* pSelectionEvent = pEvent->get<SelectionSquareEvent>();
				Vector2 vMin = pSelectionEvent->getMin();
				Vector2 vMax = pSelectionEvent->getMax();
				if (!pSelectionEvent->isActive())
				{
					SelectedTiles.clear();
					//	fill square

					//	draw lerped line
					if (nCurrentBuildMode == 0)
					{
						Vector2 vStart = pSelectionEvent->getStart();
						Vector2 vStop = pSelectionEvent->getStop();
						for (float t = 0; t < 1; t = t + 0.01)
						{
							Vector2 vLerp = lerp(vStart, vStop, t);
							if (BuildingTile* pTile = getWorldTile(vLerp.X, vLerp.Y))
								pTile->setValue(1);
						}
					}

					//	draw square
					if (nCurrentBuildMode == 1)
					{
						//	transform from world coord to tilemap coords
						Vector2 vMinTest = vMin - Position;
						Vector2 vMaxTest = vMax - Position;

						if (isValidPlacement(vMinTest, vMaxTest))
						{
							//	add building to list
							//	building position relative to tilemap at (0, 0)
							Buildings.push_back(new Building(vMinTest, vMaxTest));

							//	move to building object:  
							//	set building object to building map for rendering
							//	current floor will change
							for (int worldY = vMin.Y; worldY < vMax.Y; worldY++)
							{
								for (int worldX = vMin.X; worldX < vMax.X; worldX++)
								{
									//if (x == (int)vMin.X || x >= (int)vMax.X || y == (int)vMin.Y || y >= (int)vMax.Y)
										if (BuildingTile* pTile = getWorldTile(worldX, worldY))
											pTile->setValue(1);
										//	create building:
										//	if valid building placement
								}
							}
						}
					}

				}
				else
				{
					//	square not set/hovering
					SelectedTiles.clear();
					for (int y = vMin.Y; y < vMax.Y; y++)
					{
						for (int x = vMin.X; x < vMax.X; x++)
						{
							//	if tile is overlapping building
							//	add to red blocking tiles
							//	else
							//	add to valid tiles
							if (BuildingTile* pTile = getWorldTile(x, y))
								SelectedTiles.push_back(pTile);
						}
					}
				}
				break;
			}
		}
	}

public:
	BuildingMap() :
		nCurrentBuildMode(1),
		_TileMap({ 100, 100 }, "BUILDING_MAP") 
	{
		setPosition(5, 5);
		createBoarderMap();
		registerListener(GUI_EVENT);
		registerListener(SELECTIONSQUARE_EVENT);
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		Vector2 vTileMin = clipMin(vWorldMin);
		Vector2 vTileMax = clipMax(vWorldMax);

		int TilesRendered = 0;

		//	render clipped part of map
		for (int nY = vTileMin.Y; nY < vTileMax.Y; nY++)
			for (int nX = vTileMin.X; nX < vTileMax.X; nX++)
			{
				float fTileSize = 1.0;
				float fScaledTileSize = fTileSize * vCameraPosition.Z;

				Vector2 Min(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z),
					vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z));

				Vector2 Max(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z) + fScaledTileSize,
					vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z) + fScaledTileSize);

				pRenderer->FillQuad(Min.X,
					Min.Y,
					Max.X,
					Max.Y,
					pRenderer->getGreyscaleColor(getTile(nX, nY)->getValue()));

				TilesRendered++;
			}

		//	move to bottom of render, to put on top
		if (getMouseOverTile())
		{
			float fTileSize = 1.0;
			float fScaledTileSize = fTileSize * vCameraPosition.Z;
			int nX = getMouseOverTile()->getPosition().X;
			int nY = getMouseOverTile()->getPosition().Y;
			Vector2 Min(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z),
				vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z));

			Vector2 Max(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z) + fScaledTileSize,
				vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z) + fScaledTileSize);

			pRenderer->DrawQuad(Min.X,
				Min.Y,
				Max.X,
				Max.Y,
				Pixel(PIXEL_SOLID, FG_LIGHTGREEN));
		}


		for (auto t : SelectedTiles)
		{
			float fTileSize = 1.0;
			float fScaledTileSize = fTileSize * vCameraPosition.Z;
			int nX = t->getPosition().X;
			int nY = t->getPosition().Y;

			Vector2 Min(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z),
				vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z));

			Vector2 Max(vCameraPosition.X + ((nX + Position.X) * vCameraPosition.Z) + fScaledTileSize,
				vCameraPosition.Y + ((nY + Position.Y) * vCameraPosition.Z) + fScaledTileSize);

			pRenderer->FillQuad(Min.X,
				Min.Y,
				Max.X,
				Max.Y,
				{PIXEL_SOLID, FG_LIGHTGREEN});

			
			pRenderer->DrawQuad(Min.X,
				Min.Y,
				Max.X,
				Max.Y,
				Pixel(PIXEL_SOLID, FG_BLACK));
		}

		pRenderer->DrawNum<int>(TilesRendered, 2, pRenderer->getSize().Y - 3, FG_WHITE);
		pRenderer->DrawNum<int>(Buildings.size(), 2, pRenderer->getSize().Y - 2, FG_WHITE);

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
		registerListener(MOUSEWORLD_EVENT);
		registerListener(KEYBOARD_EVENT);
		registerListener(GUI_EVENT);
	};


	void start(BaseNode* pData, BaseNode* pSystems, BaseNode* pGUI)
	{
		CameraViewWindow* pCameraWindow = new CameraViewWindow(150, 100, 0, 0);
		pData->add(pCameraWindow->getCamera());
		pGUI->addChild(pCameraWindow);

		//SingleSelectButtonComponent* pComponent = new SingleSelectButtonComponent();
		//pComponent->addComponent(new UIButton("POINT"));
		//pComponent->addComponent(new UIButton("QUAD"));
		//pComponent->addComponent(new UIButton("CURVE"));
		//pComponent->refresh();
		//pCameraWindow->addComponent(pComponent);
		//pComponent->setAlignment(ALIGN_RIGHT);

		pData->add(new BuildingMap());
	}

	void render(OutputBuffer* pEngineBuffer)
	{
		Render2D renderer(pEngineBuffer);
	}

};