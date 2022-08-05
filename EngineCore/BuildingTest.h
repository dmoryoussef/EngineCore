

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
		bMouseOver(false),
		Min(min),
		Max(max)
	{};

	bool bMouseOver;
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
	vector<BuildingTile*> ValidTiles;
	vector<BuildingTile*> InvalidTiles;
	vector<Building*> Buildings;

	bool isValidPlacement(Vector2 vMin, Vector2 vMax)
	{
		for (auto b : Buildings)
		{
			//	create overlap quad
			//	use to fill invalid tile list
			if (vMax.X > b->Min.X - Position.X)
				if (vMin.X < b->Max.X - Position.X)
				{
					//	calc x axis overlap
					if (vMax.Y > b->Min.Y - Position.Y)
						if (vMin.Y < b->Max.Y - Position.Y)
						{
							//InvalidTiles.clear();
							////	calc y axis overlap
							//float fMinX = b->Min.X;
							//if (fMinX < vMax.X)
							//	fMinX = vMax.X;
							//float fMinY = b->Min.Y;
							//if (fMinY < vMax.Y)
							//	fMinY = vMax.Y;
							//Vector2 vOverlapMin(fMinX, fMinY);

							//float fMaxX = b->Min.X;
							//if (fMinX < vMax.X)
							//	fMaxX = vMax.X;
							//float fMaxY = b->Min.Y;
							//if (fMinY < vMax.Y)
							//	fMaxY = vMax.Y;
							//Vector2 vOverlapMax(vMax.X, vMax.Y);
							//for (int worldY = vOverlapMin.Y; worldY < vOverlapMax.Y; worldY++)
							//{
							//	for (int worldX = vOverlapMin.X; worldX < vOverlapMax.X; worldX++)
							//	{
							//		if (BuildingTile* pTile = getWorldTile(worldX, worldY))
							//			InvalidTiles.push_back(pTile);
							//	}
							//}

							return false;
						}
				}
					
		}

		return true;
	}

	void mouseOverBuildings(Vector2 mouseWorldPosition)
	{
		for (auto b : Buildings)
		{
			b->bMouseOver = false;
			if (mouseWorldPosition.X > b->Min.X &&
				mouseWorldPosition.X < b->Max.X &&
				mouseWorldPosition.Y > b->Min.Y &&
				mouseWorldPosition.Y < b->Max.Y)
				b->bMouseOver = true;
		}
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
				
				mouseOverBuildings(pMouseEvent->getWorldPosition());
				
				if (pMouseEvent->getState().bRightButtonDown)
				{
					//	undo build tile
					if (BuildingTile* pTile = getWorldTile(pMouseEvent->getWorldPosition().X, pMouseEvent->getWorldPosition().Y))
					{
						if (pTile->getValue() > 0.2)
						{
							pTile->setValue(0.1);
						}
					}
				}
				break;
			}

			case SELECTIONSQUARE_EVENT:
			{
				SelectionSquareEvent* pSelectionEvent = pEvent->get<SelectionSquareEvent>();
				Vector2 vWorldMin = pSelectionEvent->getMin();
				Vector2 vWorldMax = pSelectionEvent->getMax();
				if (!pSelectionEvent->isActive())
				{
					ValidTiles.clear();
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
						Vector2 vMinTest = vWorldMin - Position;
						Vector2 vMaxTest = vWorldMax - Position;

						if (isValidPlacement(vMinTest, vMaxTest))
						{
							//	add building to list
							Buildings.push_back(new Building(vWorldMin, vWorldMax));

							//	move to building object:  
							//	set building object to building map for rendering
							//	current floor will change
							for (int worldY = vWorldMin.Y; worldY < vWorldMax.Y; worldY++)
							{
								for (int worldX = vWorldMin.X; worldX < vWorldMax.X; worldX++)
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
					ValidTiles.clear();
					for (int y = vWorldMin.Y; y < vWorldMax.Y; y++)
					{
						for (int x = vWorldMin.X; x < vWorldMax.X; x++)
						{
							//	if tile is overlapping building
							//	add to red blocking tiles
							//	else
							//	add to valid tiles
							if (BuildingTile* pTile = getWorldTile(x, y))
								ValidTiles.push_back(pTile);
						}
					}
					isValidPlacement(vWorldMin, vWorldMax);
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

	Vector2 vScaledMin(int nX, int nY, Vector3 vCamera)
	{
		return Vector2(vCamera.X + ((nX + Position.X) * vCamera.Z),
			vCamera.Y + ((nY + Position.Y) * vCamera.Z));
	}

	Vector2 vScaledMax(int nX, int nY, float fScaledTile, Vector3 vCamera)
	{
		return Vector2(vCamera.X + ((nX + Position.X) * vCamera.Z) + fScaledTile,
			vCamera.Y + ((nY + Position.Y) * vCamera.Z) + fScaledTile);
	}

	void render(Render2D* pRenderer, Vector3 vCameraPosition, Vector2 vWorldMin, Vector2 vWorldMax)
	{
		float fTileSize = 1.0;
		int TilesRendered = 0;

		Vector2 vClippedWorldMin = clipMin(vWorldMin);
		Vector2 vClippedWorldMax = clipMax(vWorldMax);
		float fScaledTileSize = fTileSize * vCameraPosition.Z;

		//	render clipped part of map
		for (int nY = vClippedWorldMin.Y; nY < vClippedWorldMax.Y; nY++)
			for (int nX = vClippedWorldMin.X; nX < vClippedWorldMax.X; nX++)
			{
				float fTileSize = 1.0;

				Vector2 vTileMin = vScaledMin(nX, nY, vCameraPosition);
				Vector2 vTileMax = vScaledMax(nX, nY, fScaledTileSize, vCameraPosition);

				pRenderer->FillQuad(vTileMin.X,
									vTileMin.Y,
									vTileMax.X,
									vTileMax.Y,
									pRenderer->getGreyscaleColor(getTile(nX, nY)->getValue()));

				TilesRendered++;
			}

		//	move to bottom of render, to put on top
		if (getMouseOverTile())
		{
			int nX = getMouseOverTile()->getPosition().X;
			int nY = getMouseOverTile()->getPosition().Y;
			Vector2 vTileMin = vScaledMin(nX, nY, vCameraPosition);
			Vector2 vTileMax = vScaledMax(nX, nY, fScaledTileSize, vCameraPosition);

			pRenderer->DrawQuad(vTileMin.X,
								vTileMin.Y,
								vTileMax.X,
								vTileMax.Y,
								Pixel(PIXEL_SOLID, FG_LIGHTGREEN));
		}


		for (auto t : ValidTiles)
		{
			int nX = t->getPosition().X;
			int nY = t->getPosition().Y;
			Vector2 vTileMin = vScaledMin(nX, nY, vCameraPosition);
			Vector2 vTileMax = vScaledMax(nX, nY, fScaledTileSize, vCameraPosition);

			pRenderer->FillQuad(vTileMin.X,
								vTileMin.Y,
								vTileMax.X,
								vTileMax.Y,
								{PIXEL_SOLID, FG_LIGHTGREEN});
			pRenderer->DrawQuad(vTileMin.X,
								vTileMin.Y,
								vTileMax.X,
								vTileMax.Y,
								Pixel(PIXEL_SOLID, FG_BLACK));
		}

		/*for (auto t : InvalidTiles)
		{
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
				{ PIXEL_SOLID, FG_LIGHTRED });


			pRenderer->DrawQuad(Min.X,
				Min.Y,
				Max.X,
				Max.Y,
				Pixel(PIXEL_SOLID, FG_BLACK));
		}*/

		//	render mouse over building outline
		for (auto b : Buildings)
		{
			if (b->bMouseOver == true)
			{
				//	buildings are in world space already
				//	vScaled functions go from tile space to world space
				//	subtract position to normalize
				Vector2 vOutlineMin = b->Min - Position;
				Vector2 vOutlineMax = b->Max - Position;
				Vector2 vTileMin = vScaledMin(vOutlineMin.X, vOutlineMin.Y, vCameraPosition);
				Vector2 vTileMax = vScaledMax(vOutlineMax.X, vOutlineMax.Y, fScaledTileSize, vCameraPosition);

				pRenderer->DrawQuad(vTileMin.X, vTileMin.Y, vTileMax.X, vTileMax.Y, { PIXEL_SOLID, FG_LIGHTBLUE });
			}
		}

		pRenderer->DrawNum<int>(TilesRendered, 2, pRenderer->getSize().Y - 4, FG_WHITE);
		pRenderer->DrawNum<int>(Buildings.size(), 2, pRenderer->getSize().Y - 3, FG_WHITE);

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